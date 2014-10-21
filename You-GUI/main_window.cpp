//@author A0094446X
#include "stdafx.h"
#include <functional>
#include <QApplication>
#include <QList>
#include "session_manager.h"
#include "task_panel_manager.h"
#include "system_tray_manager.h"
#include "query_manager.h"
#include "main_window_messages.h"
#include "You-Controller/exception.h"
#include "You-Utils/exceptions/query_engine_exception.h"
#include "You-NLP/exception.h"

namespace You {
namespace GUI {

using Task = You::Controller::Task;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), sm(new MainWindow::SessionManager(this)),
		stm(new MainWindow::SystemTrayManager(this)),
		tpm(new MainWindow::TaskPanelManager(this)),
		qm(new MainWindow::QueryManager(this)),
		taskList(new TaskList) {
	#pragma warning(push)
	#pragma warning(disable: 4127)
	Q_INIT_RESOURCE(yougui);
	#pragma warning(pop)
	ui.setupUi(this);
	ui.menuBar->setVisible(false);
	ui.mainToolBar->setVisible(false);
	stm->setup();
	qm->setup();
	tpm->setup();
	sm->setup();
	initializeAllTimerNotifications();

	ui.commandTextBox->installEventFilter(this);
	ui.commandTextBox->setTabChangesFocus(true);
	ui.commandTextBox->setWordWrapMode(QTextOption::NoWrap);
	ui.commandTextBox->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.commandTextBox->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.commandTextBox->setFixedHeight(20);
	ui.commandTextBox->setFocus(Qt::FocusReason::ActiveWindowFocusReason);

	syntaxHighlighter.reset(
		new SyntaxHighlighter(ui.commandTextBox->document()));


	populateTaskPanel();
}

MainWindow::~MainWindow() {
}

void MainWindow::closeEvent(QCloseEvent *event) {
	if (stm->trayIcon.isVisible()) {
		QMessageBox::information(this, tr("Systray"),
			tr("The program will keep running in the "
			"system tray. To terminate the program, "
			"choose <b>Quit</b> in the context menu "
			"of the system tray entry."));
		hide();
		event->ignore();
	}
}

void MainWindow::populateTaskPanel() {
	// Grabs tasks from last session from the list of IDs saved
	QList<Task::ID> IDs = sm->taskIDs;
	TaskList tl;

	if (IDs.size() != 0) {
		tl = qm->getTasks(IDs);
	} else {
		tl = qm->getTasks();
	}
	addTasks(tl);
}

void MainWindow::setVisible(bool visible) {
	stm->minimizeAction->setEnabled(visible);
	stm->maximizeAction->setEnabled(!isMaximized());
	stm->restoreAction->setEnabled(isMaximized() || !visible);
	QWidget::setVisible(visible);
}

const TaskList& MainWindow::getTaskList() const {
	return *taskList;
}

void MainWindow::addTask(const Task& task) {
	initializeSingleTimerNotification(task);
	taskList->push_back(task);
	tpm->addTask(task);
}

void MainWindow::addTasks(const TaskList& tl) {
	std::for_each(tl.begin(), tl.end(),
		std::bind(&MainWindow::addTask, this, std::placeholders::_1));
}

void MainWindow::deleteTask(Task::ID taskID) {
	TaskList::iterator i = std::find_if(taskList->begin(), taskList->end(),
		[=](Task& task) {
			return task.getID() == taskID;
		});

	assert(i != taskList->end());
	taskList->erase(i);
	timerMap.erase(taskID);
	tpm->deleteTask(taskID);
}

void MainWindow::editTask(const Task& task) {
	timerMap.erase(task.getID());
	initializeSingleTimerNotification(task);
	tpm->editTask(task);
	ui.taskTreePanel->viewport()->update();
}

void MainWindow::sendQuery() {
	QString inputString = ui.commandTextBox->toPlainText();
	QPixmap pixmap;
	pixmap.fill(Qt::transparent);
	pixmap.load(RESOURCE_GREEN, 0);
	QString message(READY_MESSAGE);
	ui.statusMessage->setText(message);
	try {
		qm->query(inputString, getTaskList());
	} catch (You::QueryEngine::Exception::EmptyTaskDescriptionException& e) {
		ui.statusMessage->setText(EMPTY_TASK_DESCRIPTION_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch(You::QueryEngine::Exception::TaskNotFoundException& e) {
		ui.statusMessage->setText(TASK_NOT_FOUND_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::NLP::ParseErrorException& e) {
		ui.statusMessage->setText(PARSE_ERROR_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::NLP::ParserException& e) {
		ui.statusMessage->setText(PARSER_EXCEPTION_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::ContextIndexOutOfRangeException& e) {
		ui.statusMessage->setText(CONTEXT_INDEX_OUT_OF_RANGE_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::ContextRequiredException& e) {
		ui.statusMessage->setText(CONTEXT_REQUIRED_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (...) {
		ui.statusMessage->setText(UNKNOWN_EXCEPTION_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	}
	ui.statusIcon->setPixmap(pixmap);
	ui.commandTextBox->setPlainText(QString());
}

void MainWindow::commandEnterPressed() {
	sendQuery();
}

void MainWindow::commandEnterButtonClicked() {
	sendQuery();
}

void MainWindow::applicationExitRequested() {
	sm->taskIDs.clear();
	for (int i = 0; i < taskList->size(); i++) {
		sm->taskIDs.push_back(taskList->at(i).getID());
	}
	qApp->quit();
}

void MainWindow::resizeEvent(QResizeEvent* event) {
	double oldWidth = event->oldSize().width();
	double newWidth = event->size().width();
	double ratio = newWidth / oldWidth;
	for (int i = 0; i < ui.taskTreePanel->columnCount(); ++i) {
		double currWidth = ui.taskTreePanel->header()->sectionSize(i);
		double finalWidth = currWidth * ratio;
		if (finalWidth >75)
			ui.taskTreePanel->header()->resizeSection(i, currWidth * ratio);
	}
	QMainWindow::resizeEvent(event);
}

void MainWindow::clearTasks() {
	taskList.reset(new TaskList);
	ui.taskTreePanel->clear();
	ui.taskDescriptor->clear();
	ui.commandTextBox->clear();
}

void MainWindow::taskSelected() {
	QList<QTreeWidgetItem*> selection = ui.taskTreePanel->selectedItems();
	QString contents = "";
	if (selection.size() == 0) {
		ui.taskDescriptor->setText(contents);
	} else {
		QTreeWidgetItem item = *selection.at(0);
		QString index = item.text(1);
		QString description = item.text(2);
		QString deadline = item.text(3);
		QString priority = item.text(4);
		QString dependencies = item.text(5);
		contents = "Index: " + index + "\n" + "Description: " + description
			+ "\n" + "Deadline: " + deadline + "\n" + "Priority: " + priority
			+ "\n" + "Dependencies: " + dependencies;
		ui.taskDescriptor->setText(contents);
	}
}

void MainWindow::initializeAllTimerNotifications() {
	for (int i = 0; i < taskList->size(); i++) {
		initializeSingleTimerNotification(taskList->at(i));
	}
}

void MainWindow::initializeSingleTimerNotification(Task task) {
	Task::Time deadline = task.getDeadline();
	if (deadline != Task::DEFAULT_DEADLINE) {
		Task::Time now = boost::posix_time::second_clock::local_time();
		boost::posix_time::time_duration difference = deadline - now;
		QTimer* timer = new QTimer(this);
		connect(timer, SIGNAL(timeout()), this,
			SLOT(notify(notify(task.getID(), task.getDeadline()))));
		timerMap[task.getID()] = timer;
	}
}

void MainWindow::notify(Task::ID id) {
	for (auto it = taskList->begin(); it != taskList->end(); it++) {
		Task task = *it;
		if (task.getID() == id) {
			stm->trayIcon.showMessage("lol", "lol");
			break;
		}
	}
}

void MainWindow::contextAddTask() {
	ui.commandTextBox->setPlainText(QString("/add "));
	ui.commandTextBox->setFocus();
	ui.commandTextBox->moveCursor(QTextCursor::End);
}

void MainWindow::contextDeleteTask(int id) {
	std::wstringstream wss;
	wss << L"/delete " << id;
	ui.commandTextBox->setPlainText(QString::fromStdWString(wss.str()));
	ui.commandTextBox->setFocus();
	ui.commandTextBox->moveCursor(QTextCursor::End);
}

void MainWindow::contextEditTask(int id) {
	std::wstringstream wss;
	wss << L"/edit " << id << L" set ";
	ui.commandTextBox->setPlainText(QString::fromStdWString(wss.str()));
	ui.commandTextBox->setFocus();
	ui.commandTextBox->moveCursor(QTextCursor::End);
}

MainWindow::BaseManager::BaseManager(MainWindow* parentGUI)
	: parentGUI(parentGUI) {
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
	if (object == ui.commandTextBox && event->type() == QEvent::KeyPress) {
		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		if (keyEvent->key() == Qt::Key_Return) {
			commandEnterPressed();
			ui.commandTextBox->setFocus();
			return true;
		} else {
			return QMainWindow::eventFilter(object, event);
		}
	} else {
		return QMainWindow::eventFilter(object, event);
	}
}

}  // namespace GUI
}  // namespace You

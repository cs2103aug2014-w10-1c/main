//@author A0094446X
#include "stdafx.h"
#include <functional>
#include <QApplication>
#include <QList>
#include <set>
#include "session_manager.h"
#include "task_panel_manager.h"
#include "system_tray_manager.h"
#include "query_manager.h"
#include "main_window_messages.h"
#include "window_title.h"
#include "keywords.h"
#include "You-Controller/exception.h"

namespace You {
namespace GUI {

using Task = You::Controller::Task;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), sm(new MainWindow::SessionManager(this)),
		stm(new MainWindow::SystemTrayManager(this)),
		tpm(new MainWindow::TaskPanelManager(this)),
		qm(new MainWindow::QueryManager(this)),
		commandTextBox(new CommandTextBox(this)),
		taskList(new TaskList) {
	#pragma warning(push)
	#pragma warning(disable: 4127)
	Q_INIT_RESOURCE(yougui);
	#pragma warning(pop)
	ui.setupUi(this);
	ui.menuBar->setVisible(false);
	ui.mainToolBar->setVisible(false);
	setWindowTitle(QString::fromStdWString(WINDOW_TITLE));
	stm->setup();
	qm->setup();
	tpm->setup();
	sm->setup();
	ui.horizontalLayout->insertWidget(0, &*commandTextBox);
	commandTextBox->setup();
	connect(&*commandTextBox, SIGNAL(enterKey()),
		this, SLOT(commandEnterPressed()));
	connect(this, SIGNAL(updateRowNumbers()), &*tpm, SLOT(updateRowNumbers()));
	populateTaskPanel();
	statusBar()->insertPermanentWidget(
		0, ui.statusTasks, 0);
	updateTaskInfoBar();
	ui.taskDescriptor->setOpenLinks(false);
	ui.taskDescriptor->setOpenExternalLinks(false);
	connect(ui.taskDescriptor, SIGNAL(anchorClicked(const QUrl &)),
		this, SLOT(openURL(const QUrl &)));
}

MainWindow::~MainWindow() {
}

void MainWindow::populateTaskPanel() {
	StatusUpdate update;
	try {
		TaskList tl = qm->getTasks();
		addTasks(tl);
	} catch (You::Controller::IOException& e) {
		update.setUpdate(RESOURCE_RED, QString("Error loading tasks."), &ui);
		showErrorAndExit(IO_EXCEPTION_MESSAGE,
			QString("Exception: File could not be opened."));
	} catch (You::Controller::NotWellFormedXmlException& e) {
		update.setUpdate(RESOURCE_RED, QString("Error loading tasks."), &ui);
		showErrorAndExit(NOT_WELL_FORMED_XML_MESSAGE,
			QString("Exception: XML is not well formed"));
	}
	tpm->repaintTasks();
	updateRowNumbers();
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

void MainWindow::addTaskWithSubtasks(const Task& task, const TaskList& tl) {
	QTreeWidgetItem *item = tpm->addTaskTree(task).release();
	ui.taskTreePanel->addTopLevelItem(item);
	expandAllSubtasks(item);
}

void MainWindow::expandAllSubtasks(QTreeWidgetItem *item) {
	if (item->childCount() == 0) {
		return;
	} else {
		item->setExpanded(true);
		for (int i = 0; i < item->childCount(); i++) {
			expandAllSubtasks(item->child(i));
		}
	}
}

void MainWindow::addTasks(const TaskList& tl) {
	std::for_each(tl.begin(), tl.end(),
		std::bind(&MainWindow::addTaskWithSubtasks, this, std::placeholders::_1, tl));
}

void MainWindow::deleteTask(Task::ID taskID) {
	TaskList::iterator i = std::find_if(taskList->begin(), taskList->end(),
		[=](Task& task) {
			return task.getID() == taskID;
		});

	assert(i != taskList->end());
	taskList->erase(i);
	tpm->deleteTask(taskID);
}

void MainWindow::editTask(const Task& task) {
	tpm->editTask(task);
	ui.taskTreePanel->viewport()->update();
	emit(taskSelected());
}

void MainWindow::sendQuery() {
	Task::ID curr = getSelectedTaskID();
	QString inputString = commandTextBox->toPlainText();
	QPixmap pixmap;
	pixmap.fill(Qt::transparent);
	pixmap.load(RESOURCE_GREEN, 0);
	QString message(READY_MESSAGE);
	ui.statusMessage->setText(message);
	try {
		qm->query(inputString, getTaskList());
	} catch (You::Controller::EmptyTaskDescriptionException& e) {
		ui.statusMessage->setText(EMPTY_TASK_DESCRIPTION_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch(You::Controller::TaskNotFoundException& e) {
		ui.statusMessage->setText(TASK_NOT_FOUND_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::ParseErrorException& e) {
		std::string str((e.getMessage().tag));
		ui.statusMessage->setText(PARSE_ERROR_MESSAGE +
			"\"" + QString::fromStdWString(e.getToken()) +
			"\". Expected: " + QString::fromStdString(str));
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::ContextIndexOutOfRangeException& e) {
		ui.statusMessage->setText(CONTEXT_INDEX_OUT_OF_RANGE_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::ContextRequiredException& e) {
		ui.statusMessage->setText(CONTEXT_REQUIRED_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::CircularDependencyException& e) {
		ui.statusMessage->setText(CIRCULAR_DEPENDENCY_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::NotUndoAbleException& e) {
		ui.statusMessage->setText(NOT_UNDOABLE_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::ParserTypeException& e) {
		ui.statusMessage->setText(PARSER_TYPE_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	} catch (You::Controller::ParserException& e) {
		ui.statusMessage->setText(PARSER_EXCEPTION_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	}

	/// Selects the item if it was originally selected and still exists
	QList<QTreeWidgetItem*> items = ui.taskTreePanel->findItems(
		boost::lexical_cast<QString>(curr), 0);
	if (items.size() != 0) {
		if (items.size() == 1) {
			ui.taskTreePanel->setCurrentItem(items.at(0));
		} else {
			assert(false);
		}
	}

	ui.statusIcon->setPixmap(pixmap);
	commandTextBox->setPlainText(QString());
	updateTaskInfoBar();
	updateRowNumbers();
	tpm->repaintTasks();
}

void MainWindow::commandEnterPressed() {
	sendQuery();
}

void MainWindow::commandEnterButtonClicked() {
	sendQuery();
}

void MainWindow::clearTasks() {
	taskList.reset(new TaskList);
	ui.taskTreePanel->clear();
	ui.taskDescriptor->clear();
	commandTextBox->clear();
	updateTaskInfoBar();
}

void MainWindow::taskSelected() {
	/// Find selected item and fill task box
	QList<QTreeWidgetItem*> selection = ui.taskTreePanel->selectedItems();
	QString contents = "";
	if (selection.size() == 0) {
		ui.taskDescriptor->setHtml(contents);
	} else {
		QTreeWidgetItem item = *selection.at(0);
		QString index = tpm->getIndexAsText(item);
		QString description = tpm->getDescriptionAsText(item);
		QString startdate = tpm->getStartDateAsText(item);
		QString deadline = tpm->getDeadlineAsText(item);
		QString priority = tpm->getPriorityAsText(item);
		QString dependencies = tpm->getDependenciesAsText(item);
		QString attachment = tpm->getAttachmentAsText(item);
		contents = "Index: " + index + "<br />"
			+ "Description: " + description + "<br />"
			+ "Start date: " + startdate + "<br />"
			+ "Deadline: " + deadline + "<br />"
			+ "Priority: " + priority + "<br />"
			+ "Dependencies: " + dependencies + "<br / >"
			+ "Attachment: " + "<a href='" + attachment + "'>"
			+ attachment + "</a></li></ul>";
		ui.taskDescriptor->setHtml(contents);
	}
	tpm->repaintTasks();
}

Task::ID MainWindow::getSelectedTaskID() {
	QList<QTreeWidgetItem*> selection = ui.taskTreePanel->selectedItems();
	QString contents = "";
	if (selection.size() == 0) {
		return -1;
	} else {
		QTreeWidgetItem item = *selection.at(0);
		Task::ID index =
			boost::lexical_cast<Task::ID>(
			tpm->getHiddenIDAsText(item).toLongLong());
		return index;
	}
}

void MainWindow::contextAddTask() {
	commandTextBox->setPlainText(QString("/add "));
	commandTextBox->setFocus();
	commandTextBox->moveCursor(QTextCursor::End);
}

void MainWindow::contextDeleteTask(int id) {
	std::wstringstream wss;
	wss << L"/delete " << id;
	commandTextBox->setPlainText(QString::fromStdWString(wss.str()));
	commandTextBox->setFocus();
	commandTextBox->moveCursor(QTextCursor::End);
}

void MainWindow::contextEditTask(int id) {
	std::wstringstream wss;
	wss << L"/edit " << id << L" set ";
	commandTextBox->setPlainText(QString::fromStdWString(wss.str()));
	commandTextBox->setFocus();
	commandTextBox->moveCursor(QTextCursor::End);
}

void MainWindow::updateTaskInfoBar() {
	int dueSoon = 0;
	int overdue = 0;
	for (int i = 0; i < taskList->size(); i++) {
		if (tpm->isDueWithinExactly(taskList->at(i).getDeadline(), 7)) {
			dueSoon++;
		}
		if (tpm->isPastDue(taskList->at(i).getDeadline())
			&& !taskList->at(i).isCompleted()) {
			overdue++;
		}
	}
	std::wostringstream ss;
	ss << L"Overdue tasks: " << overdue << L". Tasks due soon: " << dueSoon;
	ui.statusTasks->setText(QString::fromStdWString(ss.str()));
}

void MainWindow::applicationExitRequested() {
	qApp->quit();
}

void MainWindow::changeEvent(QEvent *event) {
	if (event->type() == QEvent::WindowStateChange) {
		if (isMinimized()) {
		hide();
		}
	}
	QMainWindow::changeEvent(event);
}

void MainWindow::openURL(const QUrl &url) {
	QDesktopServices::openUrl(url);
}

void MainWindow::showErrorAndExit(QString message, QString title) {
	QMessageBox messageBox;
	messageBox.setIcon(QMessageBox::Critical);
	messageBox.setWindowTitle(title);
	messageBox.setText(message);
	messageBox.setStandardButtons(QMessageBox::Ok);
	messageBox.exec();
	QTimer::singleShot(250, qApp, SLOT(quit()));
}

MainWindow::BaseManager::BaseManager(MainWindow* parentGUI)
	: parentGUI(parentGUI) {
}

}  // namespace GUI
}  // namespace You

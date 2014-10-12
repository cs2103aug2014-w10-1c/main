//@author A0094446X
#include "stdafx.h"
#include <functional>
#include <QApplication>
#include <QList>
#include "session_manager.h"
#include "task_panel_manager.h"
#include "system_tray_manager.h"
#include "query_manager.h"
#include "you_main_gui_messages.h"
#include "You-Controller\exception.h"
#include "You-Utils\exceptions\query_engine_exception.h"
#include "You-NLP\exception.h"

using Task = You::Controller::Task;
using Result = You::Controller::Result;
using TaskList = You::Controller::TaskList;
using Controller = You::Controller::Controller;

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent), sm(new YouMainGUI::SessionManager(this)),
		stm(new YouMainGUI::SystemTrayManager(this)),
		tpm(new YouMainGUI::TaskPanelManager(this)),
		qm(new YouMainGUI::QueryManager(this)),
		taskList(new TaskList) {
	#pragma warning(push)
	#pragma warning(disable: 4127)
	Q_INIT_RESOURCE(yougui);
	#pragma warning(pop)
	ui.setupUi(this);
	stm->setup();
	qm->setup();
	tpm->setup();
	sm->setup();

	ui.commandInputBox->setFocus(Qt::FocusReason::ActiveWindowFocusReason);

	// TODO(angathorion): To fix after implementation of task handling
	populateTaskPanel();
}

YouMainGUI::~YouMainGUI() {
}

void YouMainGUI::closeEvent(QCloseEvent *event) {
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

void YouMainGUI::populateTaskPanel() {
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

void YouMainGUI::setVisible(bool visible) {
	stm->minimizeAction->setEnabled(visible);
	stm->maximizeAction->setEnabled(!isMaximized());
	stm->restoreAction->setEnabled(isMaximized() || !visible);
	QWidget::setVisible(visible);
}

const You::Controller::TaskList& YouMainGUI::getTaskList() const {
	return *taskList;
}

void YouMainGUI::addTask(const Task& task) {
	taskList->push_back(task);
	tpm->addTask(task);
}

void YouMainGUI::addTasks(const TaskList& tl) {
	std::for_each(tl.begin(), tl.end(),
		std::bind(&YouMainGUI::addTask, this, std::placeholders::_1));
}

void YouMainGUI::deleteTask(Task::ID taskID) {
	TaskList::iterator i = std::find_if(taskList->begin(), taskList->end(),
		[=](Task& task) {
			return task.getID() == taskID;
		});

	assert(i != taskList->end());
	taskList->erase(i);

	tpm->deleteTask(taskID);
}

void YouMainGUI::editTask(const Task& task) {
	tpm->editTask(task);
	ui.taskTreePanel->viewport()->update();
}

void YouMainGUI::sendQuery() {
	QString inputString = ui.commandInputBox->text();
	QPixmap pixmap;
	pixmap.fill(Qt::transparent);
	pixmap.load(RESOURCE_GREEN, 0);
	QString message(READY_MESSAGE);
	ui.statusMessage->setText(message);
	try {
		qm->query(inputString, getTaskList());
	}
	catch (You::QueryEngine::Exception::EmptyTaskDescriptionException& e) {
		ui.statusMessage->setText(EMPTY_TASK_DESCRIPTION_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	}
	catch(You::QueryEngine::Exception::TaskNotFoundException& e) {
		ui.statusMessage->setText(TASK_NOT_FOUND_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	}
	catch (You::NLP::ParseErrorException& e) {
		ui.statusMessage->setText(PARSE_ERROR_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	}
	catch (You::NLP::ParserException& e) {
		ui.statusMessage->setText(message);
		pixmap.load(RESOURCE_RED, 0);
	}
	catch (You::Controller::ContextIndexOutOfRangeException& e) {
		ui.statusMessage->setText(CONTEXT_INDEX_OUT_OF_RANGE_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	}
	catch (You::Controller::ContextRequiredException& e) {
		ui.statusMessage->setText(CONTEXT_REQUIRED_MESSAGE);
		pixmap.load(RESOURCE_RED, 0);
	}
	ui.statusIcon->setPixmap(pixmap);
	ui.commandInputBox->setText(QString());
}

void YouMainGUI::commandEnterPressed() {
	sendQuery();
}

void YouMainGUI::commandEnterButtonClicked() {
	sendQuery();
}

void YouMainGUI::applicationExitRequested() {
	sm->taskIDs.clear();
	for (int i = 0; i < taskList->size(); i++) {
		sm->taskIDs.push_back(taskList->at(i).getID());
	}
	qApp->quit();
}

YouMainGUI::BaseManager::BaseManager(YouMainGUI* parentGUI)
	: parentGUI(parentGUI) {
}

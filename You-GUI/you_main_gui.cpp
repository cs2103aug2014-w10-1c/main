//@author A0094446X
#include "stdafx.h"
#include <functional>
#include <QApplication>
#include <QList>
#include "session_manager.h"
#include "task_panel_manager.h"
#include "system_tray_manager.h"
#include "NLP_manager.h"

using Task = You::Controller::Task;
using Result = You::Controller::Result;
using TaskList = You::Controller::TaskList;
using Controller = You::Controller::Controller;

YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent), sm(new YouMainGUI::SessionManager(this)),
		stm(new YouMainGUI::SystemTrayManager(this)),
		tpm(new YouMainGUI::TaskPanelManager(this)),
		nlpm(new YouMainGUI::NLPManager(this)),
		taskList(new TaskList) {
	Q_INIT_RESOURCE(yougui);

	ui.setupUi(this);
	stm->setup();
	nlpm->setup();
	tpm->setup();
	sm->setup();

	connect(ui.commandEnterButton,
		SIGNAL(clicked()), this, SLOT(commandEnterButtonClicked()));

	// TODO(angathorion): To fix after implementation of task handling
	// populateTaskPanel();
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
		tl = nlpm->getTasks(IDs);
	} else {
		tl = nlpm->getTasks();
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
	tpm->addTask(task);
}

void YouMainGUI::addTasks(const TaskList& tl) {
	std::for_each(tl.begin(), tl.end(),
		std::bind(&YouMainGUI::addTask, this, std::placeholders::_1));
}

void YouMainGUI::deleteTask(Task::ID taskID) {
	tpm->deleteTask(taskID);
}

void YouMainGUI::editTask(const Task& task) {
	tpm->editTask(task);
}

void YouMainGUI::commandEnterButtonClicked() {
	QString inputString = ui.commandInputBox->text();

	nlpm->query(inputString, getTaskList());
	// For demonstration purposes
	// parentGUI->populateTaskPanel();
}

YouMainGUI::BaseManager::BaseManager(YouMainGUI* parentGUI)
	: parentGUI(parentGUI) {
}

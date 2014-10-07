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
	std::vector<Task::ID> IDs = sm->taskIDs;
	TaskList tl;
	if (IDs.size() != 0) {
		tl = Controller::get().getTasks(sm->taskIDs);
	} else {
		tl = Controller::get().getTasks();
	}
	addTaskListToPanel(tl);
}

void YouMainGUI::addTaskListToPanel(TaskList tl) {
	std::for_each(tl.begin(), tl.end(),
		std::bind(&YouMainGUI::addTask, this, std::placeholders::_1));
}

YouMainGUI::BaseManager::BaseManager(YouMainGUI* parentGUI)
	: parentGUI(parentGUI) {}

void YouMainGUI::setVisible(bool visible) {
	stm->minimizeAction->setEnabled(visible);
	stm->maximizeAction->setEnabled(!isMaximized());
	stm->restoreAction->setEnabled(isMaximized() || !visible);
	QWidget::setVisible(visible);
}

You::Controller::TaskList YouMainGUI::getTaskList() {
	return *taskList;
}

void YouMainGUI::addTask(You::Controller::Task task) {
	tpm->addTask(taskToStrVec(task));
}

void YouMainGUI::deleteTask(You::Controller::Task::ID task) {
	QList<QTreeWidgetItem*> items =
		ui.taskTreePanel->findItems(
		QString::fromStdWString(std::to_wstring(task)), 0);
	if (items.length() != 1) {
		qDebug() << "deleteTask items.length() != 1" << endl;
	} else {
		tpm->deleteTask(items.at(0));
	}
}

void YouMainGUI::editTask(You::Controller::Task task) {
	QList<QTreeWidgetItem*> items =
		ui.taskTreePanel->findItems(
		QString::fromStdWString(std::to_wstring(task.getID())), 0);
	if (items.length() != 1) {
		qDebug() << "editTask items.length() != 1" << endl;
	} else {
		QTreeWidgetItem item = *items.at(0);
		QStringList wstr = taskToStrVec(task);
		*items.at(0) = *tpm->createItem(wstr);
	}
}

QStringList YouMainGUI::taskToStrVec(const You::Controller::Task& task) {
	QStringList result;
	
	// Insert id
	result.push_back(boost::lexical_cast<QString>(task.getID()));

	// Insert count
	result.push_back("0");

	// Insert description
	result.push_back(QString::fromStdWString(task.getDescription()));

	// Insert deadline
	result.push_back(boost::lexical_cast<QString>(task.getDeadline()));

#if 0
	// Iterate through task list and add it to the task panel
	std::wstring priority[] { L"Important", L"Normal" };

	switch (taskList->at(i).getPriority()) {
	case Task::Priority::IMPORTANT:
		rowStrings.push_back(priority[0]);
	case Task::Priority::NORMAL:
		rowStrings.push_back(priority[1]);
	}

	// TODO(angathorion): Deal with dependencies
#endif

	return result;
}

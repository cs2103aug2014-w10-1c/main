//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "variant_handler.h"
#include "session_manager.h"
#include "task_panel_manager.h"
#include "system_tray_manager.h"
#include "NLP_manager.h"
#include <boost\date_time\gregorian\greg_month.hpp>
typedef int64_t TaskID;
YouMainGUI::YouMainGUI(QWidget *parent)
	: QMainWindow(parent), sm(new YouMainGUI::SessionManager(this)),
		stm(new YouMainGUI::SystemTrayManager(this)),
		tpm(new YouMainGUI::TaskPanelManager(this)),
		nlpm(new YouMainGUI::NLPManager(this)),
		taskList(new You::Controller::TaskList),
		vh(new VariantHandler) {
	vh->parentGUI = this;
	columnHeaders = { TASK_COLUMN_1,
		TASK_COLUMN_2, TASK_COLUMN_3, TASK_COLUMN_4, TASK_COLUMN_5 };
	ui.setupUi(this);
	stm->setup();
	nlpm->setup();
	tpm->setup();
	sm->setup();
	// To fix after implementation of task handling
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
	/* To fix
	std::vector<TaskID> IDs = sm->taskIDs;
	You::Controller::Result result;

	if (IDs.size() != 0) {
		result = You::Controller::Controller::get().getTasks(sm->taskIDs);
	} else {
		result = You::Controller::Controller::get().getTasks();
	}
	You::Controller::TaskList tl = boost::get<You::Controller::TaskList>(result);
	taskList.reset(&tl);
	// Iterate through task list and add it to the task panel
	std::wstring priority[] { L"Important", L"Normal" };
	for (int i = 0; i < 10; i++) {
		std::vector<std::wstring> rowStrings;
		rowStrings.push_back(std::to_wstring(taskList->at(i).getID()));
		rowStrings.push_back(taskList->at(i).getDescription());
		std::wstringstream wss;
		wss << taskList->at(i).getDeadline();
		rowStrings.push_back(wss.str());
		switch (taskList->at(i).getPriority()) {
		case You::Controller::Task::Priority::IMPORTANT: rowStrings.push_back(priority[0]);
		case You::Controller::Task::Priority::NORMAL: rowStrings.push_back(priority[1]);
		}
		// To do: Deal with dependencies
		tpm->addTask(rowStrings);
	}
	*/
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

std::vector<std::wstring> YouMainGUI::taskToStrVec(You::Controller::Task task) {
	// Make wstringstream as helper for conversion
	std::wstringstream wss;

	// Initialize vector
	std::vector<std::wstring> taskVector;

	// Insert id
	wss << task.getID();
	taskVector.push_back(wss.str());
	wss.str(L"");

	// Insert description
	taskVector.push_back(task.getDescription());

	// Insert deadline
	wss << task.getDeadline();
	std::wstring deadline = wss.str();
	taskVector.push_back(deadline);
	wss.str(L"");

	// To do
	/*
	for (int i = 0; i < task.getDependencies().size(), i++) {

	}
	*/
	return taskVector;
}

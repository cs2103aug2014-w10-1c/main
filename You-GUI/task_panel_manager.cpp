//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "task_panel_manager.h"
YouMainGUI::TaskPanelManager::~TaskPanelManager() {
}

void YouMainGUI::TaskPanelManager::setup() {
	parentGUI->ui.taskTreePanel->setColumnCount(4);
	parentGUI->ui.taskTreePanel->setHeaderItem(
		createItem(parentGUI->columnHeaders));
	parentGUI->ui.taskTreePanel->setColumnHidden(0, true);
}

void YouMainGUI::TaskPanelManager::addTask(
	std::vector<std::wstring> rowStrings) {
	QTreeWidgetItem* item = createItem(rowStrings);
	parentGUI->ui.taskTreePanel->addTopLevelItem(item);
}

void YouMainGUI::TaskPanelManager::addSubtask(QTreeWidgetItem* parent,
	std::vector<std::wstring> rowStrings) {
	QTreeWidgetItem* item = createItem(rowStrings);
	parent->addChild(item);
}

QTreeWidgetItem* YouMainGUI::TaskPanelManager::createItem(
	std::vector<std::wstring> rowStrings) {
	QStringList tempList;
	std::vector<std::wstring>::iterator it;
	for (auto it = rowStrings.begin(); it != rowStrings.end(); it++) {
		tempList << QString::fromStdWString(*it);
	}
	QTreeWidgetItem *item = new QTreeWidgetItem(tempList);
	return item;
}

void YouMainGUI::TaskPanelManager::deleteTask(QTreeWidgetItem* task) {
	delete task;
}

std::vector<std::wstring> taskToStrVec(You::NLP::Task task) {
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

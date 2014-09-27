#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "you_main_gui.h"

YouMainGUI::TaskPanelManager::~TaskPanelManager() {
}

void YouMainGUI::TaskPanelManager::setup() {}

void YouMainGUI::TaskPanelManager::taskPanelSetup() {
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

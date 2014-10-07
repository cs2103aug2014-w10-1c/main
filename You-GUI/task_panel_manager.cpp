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
		createItem(parentGUI->columnHeaders).release());
	// parentGUI->ui.taskTreePanel->setColumnHidden(0, true);
}

void YouMainGUI::TaskPanelManager::addTask(
	const QStringList& rowStrings) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(rowStrings));
	parentGUI->ui.taskTreePanel->addTopLevelItem(item.release());
}

void YouMainGUI::TaskPanelManager::addSubtask(QTreeWidgetItem* parent,
	const QStringList& rowStrings) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(rowStrings));
	parent->addChild(item.release());
}

std::unique_ptr<QTreeWidgetItem> YouMainGUI::TaskPanelManager::createItem(
	const QStringList& rowStrings) {
	return std::make_unique<QTreeWidgetItem>(rowStrings);
}

void YouMainGUI::TaskPanelManager::deleteTask(QTreeWidgetItem* task) {
	delete task;
}

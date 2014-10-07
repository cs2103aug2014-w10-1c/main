//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "task_panel_manager.h"

const int YouMainGUI::TaskPanelManager::TASK_COLUMN_COUNT = 4;
const QString YouMainGUI::TaskPanelManager::TASK_COLUMN_1 = "Hidden ID Column";
const QString YouMainGUI::TaskPanelManager::TASK_COLUMN_2 = "Index";
const QString YouMainGUI::TaskPanelManager::TASK_COLUMN_3 = "Description";
const QString YouMainGUI::TaskPanelManager::TASK_COLUMN_4 = "Deadline";
const QString YouMainGUI::TaskPanelManager::TASK_COLUMN_5 = "Priority";

YouMainGUI::TaskPanelManager::TaskPanelManager(YouMainGUI* const parentGUI)
: BaseManager(parentGUI) {
}
YouMainGUI::TaskPanelManager::~TaskPanelManager() {
}

void YouMainGUI::TaskPanelManager::setup() {
	QStringList columnHeaders({
		TASK_COLUMN_1,
		TASK_COLUMN_2,
		TASK_COLUMN_3,
		TASK_COLUMN_4,
		TASK_COLUMN_5,
	});

	parentGUI->ui.taskTreePanel->setColumnCount(4);
	parentGUI->ui.taskTreePanel->setHeaderItem(
		createItem(columnHeaders).release());
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

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
	const std::vector<std::wstring>& rowStrings) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(rowStrings));
	parentGUI->ui.taskTreePanel->addTopLevelItem(item.release());
}

void YouMainGUI::TaskPanelManager::addSubtask(QTreeWidgetItem* parent,
	const std::vector<std::wstring>& rowStrings) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(rowStrings));
	parent->addChild(item.release());
}

std::unique_ptr<QTreeWidgetItem> YouMainGUI::TaskPanelManager::createItem(
	const std::vector<std::wstring>& rowStrings) {
	QStringList tempList;
	std::transform(
		rowStrings.begin(),
		rowStrings.end(),
		std::back_inserter(tempList),
		[](const std::wstring& str) {
			return QString::fromStdWString(str);
		});

	return std::make_unique<QTreeWidgetItem>(tempList);
}

void YouMainGUI::TaskPanelManager::deleteTask(QTreeWidgetItem* task) {
	delete task;
}

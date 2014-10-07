//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "task_panel_manager.h"

using Task = You::Controller::Task;

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

	QTreeWidget* taskTreePanel = parentGUI->ui.taskTreePanel;
	taskTreePanel->setColumnCount(columnHeaders.size());
	taskTreePanel->setHeaderItem(createItem(columnHeaders).release());

	// TODO(angathorion): remove magic constants.
	QHeaderView* header = taskTreePanel->header();
	header->setStretchLastSection(false);
	for (int i = 1; i < columnHeaders.size(); ++i) {
		if (i == 2) {
			continue;
		}
		header->resizeSection(i, header->defaultSectionSize());
	}

	taskTreePanel->setColumnHidden(0, true);
}

void YouMainGUI::TaskPanelManager::addTask(const Task& task) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(task));
	parentGUI->ui.taskTreePanel->addTopLevelItem(item.release());
}

void YouMainGUI::TaskPanelManager::addSubtask(QTreeWidgetItem* parent,
	const QStringList& rowStrings) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(rowStrings));
	parent->addChild(item.release());
}

void YouMainGUI::TaskPanelManager::editTask(const Task& task) {
	QList<QTreeWidgetItem*> items = findItems(task.getID());

	if (items.length() != 1) {
		qDebug() << "editTask items.length() != 1" << endl;
	} else {
		QTreeWidgetItem item = *items.at(0);
		QStringList wstr = taskToStrVec(task);
		*items.at(0) = *createItem(wstr);
	}
}

void YouMainGUI::TaskPanelManager::deleteTask(Task::ID taskID) {
	QList<QTreeWidgetItem*> items = findItems(taskID);

	if (items.length() != 1) {
		qDebug() << "deleteTask items.length() != 1" << endl;
	} else {
		deleteTask(items.at(0));
	}
}

void YouMainGUI::TaskPanelManager::deleteTask(QTreeWidgetItem* task) {
	delete task;
}

std::unique_ptr<QTreeWidgetItem> YouMainGUI::TaskPanelManager::createItem(
	const Task& task) {
	return createItem(taskToStrVec(task));
}

std::unique_ptr<QTreeWidgetItem> YouMainGUI::TaskPanelManager::createItem(
	const QStringList& rowStrings) {
	return std::make_unique<QTreeWidgetItem>(rowStrings);
}

QList<QTreeWidgetItem*> YouMainGUI::TaskPanelManager::findItems(
	You::Controller::Task::ID taskID) const {
	return parentGUI->ui.taskTreePanel->findItems(
		boost::lexical_cast<QString>(taskID), 0);
}

QStringList YouMainGUI::TaskPanelManager::taskToStrVec(
	const You::Controller::Task& task) {
	QStringList result;

	// Insert id
	result.push_back(boost::lexical_cast<QString>(task.getID()));

	// Insert count
	result.push_back("0");

	// Insert description
	result.push_back(QString::fromStdWString(task.getDescription()));

	// Insert deadline
	result.push_back(boost::lexical_cast<QString>(task.getDeadline()));


	// Iterate through task list and add it to the task panel
	std::wstring priority[] { L"Important", L"Normal" };

	switch (task.getPriority()) {
	case Task::Priority::IMPORTANT:
		result.push_back(boost::lexical_cast<QString>(priority[0]));
	case Task::Priority::NORMAL:
		result.push_back(boost::lexical_cast<QString>(priority[1]));
	}

	// TODO(angathorion): Deal with dependencies


	return result;
}

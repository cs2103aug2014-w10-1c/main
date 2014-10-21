//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include <QPair>
#include "task_panel_manager.h"
namespace You {
namespace GUI {

using Date = boost::gregorian::date;
const QString MainWindow::TaskPanelManager::TASK_COLUMN_1 = "Hidden ID Column";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_2 = "Index";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_3 = "Description";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_4 = "Deadline";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_5 = "Priority";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_6 = "Dependencies";

MainWindow::TaskPanelManager::TaskPanelManager(MainWindow* const parentGUI)
: BaseManager(parentGUI), deleteAction(QString("Delete"), this),
editAction(QString("Edit"), this), addAction(QString("Add"), this),
deleteSignalMapper(this), editSignalMapper(this) {
}

MainWindow::TaskPanelManager::~TaskPanelManager() {
}

void MainWindow::TaskPanelManager::setup() {
	QStringList columnHeaders({
		TASK_COLUMN_1,
		TASK_COLUMN_2,
		TASK_COLUMN_3,
		TASK_COLUMN_4,
		TASK_COLUMN_5,
		TASK_COLUMN_6
	});
	QTreeWidget* taskTreePanel = parentGUI->ui.taskTreePanel;
	connect(taskTreePanel, SIGNAL(itemSelectionChanged()),
		parentGUI, SLOT(taskSelected()));

	connect(taskTreePanel, SIGNAL(customContextMenuRequested(const QPoint &)),
		this, SLOT(contextMenu(const QPoint &)));
	taskTreePanel->setColumnCount(columnHeaders.size());
	taskTreePanel->setHeaderItem(createItem(columnHeaders).release());
	// TODO(angathorion): remove magic constants.
	QHeaderView* header = taskTreePanel->header();
	header->setStretchLastSection(true);

	for (int i = 1; i < columnHeaders.size(); ++i) {
		if (i == 2) {
			continue;
		}
		header->resizeSection(i, header->defaultSectionSize());
	}

	taskTreePanel->header()->setMinimumSectionSize(75);
	taskTreePanel->setColumnHidden(0, true);
	taskTreePanel->setColumnHidden(5, true);
}

void MainWindow::TaskPanelManager::addTask(const Task& task) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(task));
	parentGUI->ui.taskTreePanel->addTopLevelItem(item.release());
	updateRowNumbers();
}

void MainWindow::TaskPanelManager::addSubtask(QTreeWidgetItem* parent,
	const QStringList& rowStrings) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(rowStrings));
	parent->addChild(item.release());
	updateRowNumbers();
}

void MainWindow::TaskPanelManager::editTask(const Task& task) {
	QList<QTreeWidgetItem*> items = findItems(task.getID());
	assert(items.length() == 1);
	QTreeWidgetItem item = *items.at(0);
	QStringList wstr = taskToStrVec(task);
	*items.at(0) = *createItem(wstr);
	updateRowNumbers();
}

void MainWindow::TaskPanelManager::deleteTask(Task::ID taskID) {
	QList<QTreeWidgetItem*> items = findItems(taskID);
	assert(items.length() == 1);
	deleteTask(items.at(0));
	updateRowNumbers();
}

void MainWindow::TaskPanelManager::deleteTask(QTreeWidgetItem* task) {
	delete task;
}

std::unique_ptr<QTreeWidgetItem> MainWindow::TaskPanelManager::createItem(
	const Task& task) {
	return createItem(taskToStrVec(task));
}

std::unique_ptr<QTreeWidgetItem> MainWindow::TaskPanelManager::createItem(
	const QStringList& rowStrings) {
	return std::make_unique<QTreeWidgetItem>(rowStrings);
}

QList<QTreeWidgetItem*> MainWindow::TaskPanelManager::findItems(
	You::Controller::Task::ID taskID) const {
	return parentGUI->ui.taskTreePanel->findItems(
		boost::lexical_cast<QString>(taskID), 0);
}

QStringList MainWindow::TaskPanelManager::taskToStrVec(
	const You::Controller::Task& task) {
	QStringList result;

	// Insert id
	result.push_back(boost::lexical_cast<QString>(task.getID()));

	// Insert dummy count
	result.push_back("0");

	// Insert description
	result.push_back(QString::fromStdWString(task.getDescription()));

	// Insert deadline
	std::wstringstream wss;
	if (task.getDeadline() == Task::NEVER) {
		wss << L"Never";
	} else if (isPastDue(task.getDeadline())) {
		wss << L"Overdue (" << task.getDeadline() << L")";
	} else if (isDueAfter(task.getDeadline(), 0)) {
		wss << L"Today (" << task.getDeadline() << L")";
	} else if (isDueAfter(task.getDeadline(), 1)) {
		wss << L"Tomorrow (" << task.getDeadline() << L")";
	} else if (isDueAfter(task.getDeadline(), 2)) {
		wss << L"In two days (" << task.getDeadline() << L")";
	} else if (isDueWithin(task.getDeadline(), 7)) {
		wss << L"Within one week (" << task.getDeadline() << L")";
	} else if (isDueWithin(task.getDeadline(), 14)) {
		wss << L"Within two weeks (" << task.getDeadline() << L")";
	} else if (isDueWithin(task.getDeadline(), 21)) {
		wss << L"Within three weeks (" << task.getDeadline() << L")";
	} else if (isDueWithin(task.getDeadline(), 28)) {
		wss << L"Within one month (" << task.getDeadline() << L")";
	} else {
		wss << L"More than a month away (" << task.getDeadline() << L")";
	}
	result.push_back(boost::lexical_cast<QString>(wss.str()));

	// Insert priority
	QString priority[] { "High", "Normal" };

	switch (task.getPriority()) {
	case Task::Priority::HIGH:
		result.push_back(priority[0]);
	case Task::Priority::NORMAL:
		result.push_back(priority[1]);
	}

	// Insert dependencies
	if (task.getDependencies().size() != 0) {
		Task::Dependencies dependencies = task.getDependencies();
		std::wstringstream ss;
		std::wstring comma = L"";
		for (auto iter = dependencies.begin();
			iter != dependencies.end(); ++iter) {
			ss << comma << *iter;
			comma = L", ";
		}
		result.push_back(QString::fromStdWString(ss.str()));
	} else {
		result.push_back("None");
	}

	return result;
}

void MainWindow::TaskPanelManager::updateRowNumbers() {
	int rowNum = 0;
	for (QTreeWidgetItemIterator it(parentGUI->ui.taskTreePanel); *it; ++it) {
		(*it)->setData(1, Qt::DisplayRole, rowNum++);
	}
}

bool MainWindow::TaskPanelManager::isPastDue(Task::Time deadline) {
	Task::Time now = boost::posix_time::second_clock::local_time();
	return deadline < now;
}

bool MainWindow::TaskPanelManager::isDueAfter(
		Task::Time deadline, int daysLeft) {
	Date by = Date(deadline.date());
	Date today = boost::gregorian::day_clock::local_day();
	return by.modjulian_day() - today.modjulian_day() == daysLeft;
}

bool MainWindow::TaskPanelManager::isDueWithin(
	Task::Time deadline, int daysLeft) {
	Date by = Date(deadline.date());
	Date today = boost::gregorian::day_clock::local_day();
	return by.julian_day() - today.julian_day() < daysLeft;
}

void MainWindow::TaskPanelManager::contextMenu(const QPoint &pos) {
	/// Try to get the item at the position of the context menu
	QTreeWidgetItem *item = parentGUI->ui.taskTreePanel->itemAt(pos);
	itemContextMenu.reset(new QMenu(parentGUI->ui.taskTreePanel));

	/// Adds the Add Task action. This is always present in the menu.
	itemContextMenu->addAction(&addAction);
	connect(&addAction, SIGNAL(triggered()), parentGUI, SLOT(contextAddTask()));

	/// Adds the Edit Task and Delete Task actions if an item is selected.
	/// QSignalMapper is necessary because arguments need to be passed
	/// as part of the signal.
	if (item) {
		deleteSignalMapper.setMapping(&deleteAction, item->text(1).toInt());
		connect(&deleteAction, SIGNAL(triggered()),
			&deleteSignalMapper, SLOT(map()));
		itemContextMenu->addAction(&deleteAction);
		connect(&deleteSignalMapper, SIGNAL(mapped(int)),
			parentGUI, SLOT(contextDeleteTask(int)));

		editSignalMapper.setMapping(&editAction, item->text(1).toInt());
		connect(&editAction, SIGNAL(triggered()),
			&editSignalMapper, SLOT(map()));
		itemContextMenu->addAction(&editAction);
		connect(&editSignalMapper, SIGNAL(mapped(int)),
			parentGUI, SLOT(contextEditTask(int)));
	}

	itemContextMenu->popup(
		parentGUI->ui.taskTreePanel->viewport()->mapToGlobal(pos));
}

}  // namespace GUI
}  // namespace You

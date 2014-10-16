//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
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
: BaseManager(parentGUI) {
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
	if (task.getDeadline() == Task::NEVER) {
		result.push_back(QString("Never"));
	} else {
		result.push_back(boost::lexical_cast<QString>(task.getDeadline()));
		task.getDeadline();
	}

	// Insert priority
	QString priority[] { "High", "Normal" };

	switch (task.getPriority()) {
	case Task::Priority::HIGH:
		result.push_back(priority[0]);
	case Task::Priority::NORMAL:
		result.push_back(priority[1]);
	}

	// Insert dependencies
	std::wstringstream ss;
	if (task.getDependencies().size() != 0) {
		ss << task.getDependencies().at(0);
		for (int i = 1; i < task.getDependencies().size(); i++) {
			ss << ", " << task.getDependencies().at(i);
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
	return true;
}

bool MainWindow::TaskPanelManager::isDueAfter(
		Task::Time deadline, int daysLeft) {
	Date by = Date(deadline.date());
	Date today = boost::gregorian::day_clock::local_day();
	if (by.day_of_year() - today.day_of_year() == daysLeft) {
		return true;
	} else {
		return false;
	}
}


}  // namespace GUI
}  // namespace You

//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include <QPair>
#include <QBrush>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include "task_panel_manager.h"
namespace You {
namespace GUI {

using Date = boost::gregorian::date;

const QString MainWindow::TaskPanelManager::TASK_COLUMN_0_TITLE = "Index";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_1_TITLE = "Hidden ID Column";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_2_TITLE = "Description";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_3_TITLE = "Deadline";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_4_TITLE = "Priority";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_5_TITLE = "Dependencies";
const QString MainWindow::TaskPanelManager::TASK_COLUMN_6_TITLE = "Completion";

const int MainWindow::TaskPanelManager::COLUMN_INDEX = 0;
const int MainWindow::TaskPanelManager::COLUMN_HIDDEN_ID = 1;
const int MainWindow::TaskPanelManager::COLUMN_DESCRIPTION = 2;
const int MainWindow::TaskPanelManager::COLUMN_DEADLINE = 3;
const int MainWindow::TaskPanelManager::COLUMN_PRIORITY = 4;
const int MainWindow::TaskPanelManager::COLUMN_DEPENDENCIES = 5;
const int MainWindow::TaskPanelManager::COLUMN_COMPLETION = 6;

MainWindow::TaskPanelManager::TaskPanelManager(MainWindow* const parentGUI)
: BaseManager(parentGUI), deleteAction(QString("Delete"), this),
editAction(QString("Edit"), this), addAction(QString("Add"), this),
deleteSignalMapper(this), editSignalMapper(this) {
}

MainWindow::TaskPanelManager::~TaskPanelManager() {
}

void MainWindow::TaskPanelManager::setup() {
	QStringList columnHeaders({
		TASK_COLUMN_0_TITLE,
		TASK_COLUMN_1_TITLE,
		TASK_COLUMN_2_TITLE,
		TASK_COLUMN_3_TITLE,
		TASK_COLUMN_4_TITLE,
		TASK_COLUMN_5_TITLE,
		TASK_COLUMN_6_TITLE
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

	for (int i = 0; i < columnHeaders.size(); ++i) {
		if (i == COLUMN_DESCRIPTION) {
			continue;
		}
		header->resizeSection(i, header->defaultSectionSize());
	}

	taskTreePanel->header()->setMinimumSectionSize(75);
	taskTreePanel->setColumnHidden(COLUMN_HIDDEN_ID, true);
	taskTreePanel->setColumnHidden(COLUMN_DEPENDENCIES, true);
	taskTreePanel->setColumnHidden(COLUMN_COMPLETION, true);
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

void MainWindow::TaskPanelManager::addSubtask(const Task& parentTask,
	const Task& childTask) {
	std::unique_ptr<QTreeWidgetItem> item(createItem(childTask));
	QList<QTreeWidgetItem*> items = findItems(parentTask.getID());
	items.at(0)->addChild(item.release());
	updateRowNumbers();
}

void MainWindow::TaskPanelManager::editTask(const Task& task) {
	QList<QTreeWidgetItem*> items = findItems(task.getID());
	assert(items.length() == 1);
	QTreeWidgetItem item = *items.at(0);
	QStringList wstr = taskToStrVec(task);
	*items.at(0) = *createItem(wstr);
	if (task.isCompleted()) {
		for (int i = 0; i < items.at(0)->columnCount(); i++) {
			QFont font = (items.at(0)->font(i));
			QBrush brush(Qt::gray);
			font.setStrikeOut(true);
			items.at(0)->setFont(i, font);
			items.at(0)->setForeground(i, brush);
		}
	} else {
		for (int i = 0; i < items.at(0)->columnCount(); i++) {
			QFont font = (items.at(0)->font(i));
			font.setStrikeOut(false);
			items.at(0)->setFont(i, font);
			items.at(0)->setForeground(i, Qt::black);
		}
	}
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

/// Takes a single task, a map of all tasks, and produces a QTreeWidgetItem
/// with all its subtasks, recursively.
std::unique_ptr<QTreeWidgetItem> MainWindow::TaskPanelManager::makeTree(
	const Task& task, const std::map<Task::ID, Task> taskMap) {
	parentGUI->taskList->push_back(task);
	std::unique_ptr<QTreeWidgetItem> root = createItem(task);
	for (Task::ID id : task.getSubtasks()) {
		Task subtask = taskMap.find(id)->second;
		root->addChild(makeTree(subtask, taskMap).release());
	}
	return root;
}

std::unique_ptr<QTreeWidgetItem> MainWindow::TaskPanelManager::createItem(
	const Task& task) {
	std::unique_ptr<QTreeWidgetItem> item = createItem(taskToStrVec(task));
	item->setChildIndicatorPolicy(
		QTreeWidgetItem::DontShowIndicatorWhenChildless);
	return item;
}

std::unique_ptr<QTreeWidgetItem> MainWindow::TaskPanelManager::createItem(
	const QStringList& rowStrings) {
	return std::make_unique<QTreeWidgetItem>(rowStrings);
}

QList<QTreeWidgetItem*> MainWindow::TaskPanelManager::findItems(
	You::Controller::Task::ID taskID) const {
	return parentGUI->ui.taskTreePanel->findItems(
		boost::lexical_cast<QString>(taskID), Qt::MatchRecursive, 1);
}

QStringList MainWindow::TaskPanelManager::taskToStrVec(
	const You::Controller::Task& task) {
	QStringList result;
	// Insert dummy count
	result.push_back("0");

	// Insert id
	result.push_back(boost::lexical_cast<QString>(task.getID()));

	// Insert description
	result.push_back(QString::fromStdWString(task.getDescription()));

	// Insert deadline
	result.push_back(getReadableDeadline(task));

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
		std::wstring temp = boost::algorithm::join(dependencies |
			boost::adaptors::transformed(
			static_cast<std::wstring (*)(Task::ID)>(&std::to_wstring)),
			L", ");
		result.push_back(boost::lexical_cast<QString>(temp));
	} else {
		result.push_back("None");
	}

	// Insert done-ness
	if (task.isCompleted()) {
		result.push_back(boost::lexical_cast<QString>("Yes"));
	} else {
		result.push_back(boost::lexical_cast<QString>("No"));
	}

	return result;
}

void MainWindow::TaskPanelManager::repaintTasks() {
	QTreeWidgetItemIterator it(parentGUI->ui.taskTreePanel);
	/// Iterate through all tasks
	while (*it) {
		QTreeWidgetItem *item = *it;
		/// If task is not done
		if (getCompletionAsText(*item).compare(QString("No")) == 0) {
			QList<QTreeWidgetItem*> selection = parentGUI->ui.taskTreePanel->selectedItems();
			if (selection.size() != 0) {
				/// Get selected task
				Task::ID id = parentGUI->getSelectedTaskID();
				TaskList::iterator i = std::find_if(
					parentGUI->taskList->begin(), parentGUI->taskList->end(),
					[=](Task& task) {
					return task.getID() == id;
				});
				Task task = *i;

				/// Get ID of iterated task
				Task::ID index = boost::lexical_cast<Task::ID>(
					getHiddenIDAsText(*item).toLongLong());
				QFont font = (item->font(0));
				font.setStrikeOut(false);
				/// If current task is a dependency of selected task
				Task::Dependencies dependencies = task.getDependencies();
				if (std::find(dependencies.begin(), dependencies.end(), index)
					!= dependencies.end()) {
					colorTask(item, Qt::blue, font);
				} else {
					colorTask(item, Qt::black, font);
				}
			} else {
				/// No selection
				QFont font = (item->font(0));
				font.setStrikeOut(false);
				colorTask(item, Qt::black, font);
			}
		} else {
			/// Overrides all other formats if task is already done
			QFont font = (item->font(0));
			font.setStrikeOut(true);
			colorTask(item, Qt::gray, font);
		}
		++it;
	}
}

void MainWindow::TaskPanelManager::updateRowNumbers() {
	int rowNum = 0;
	for (QTreeWidgetItemIterator it(parentGUI->ui.taskTreePanel); *it; ++it) {
		(*it)->setData(0, Qt::DisplayRole, rowNum++);
	}
}

void MainWindow::TaskPanelManager::colorTask(
	QTreeWidgetItem *taskItem, QColor color, QFont font) {
	for (int i = 0; i < taskItem->columnCount(); i++) {
		taskItem->setTextColor(i, color);
		taskItem->setFont(i, font);
	}
}
bool MainWindow::TaskPanelManager::isPastDue(Task::Time deadline) {
	Task::Time now = boost::posix_time::second_clock::local_time();
	return deadline < now;
}

bool MainWindow::TaskPanelManager::isDueWithinExactly(
	Task::Time deadline, int days) {
	Task::Time now = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration day(days * 24, 0, 0, 0);
	return deadline < (now + day) && deadline > now;
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

QString MainWindow::TaskPanelManager::getReadableDeadline(Task task) {
	std::wstringstream wss;
	boost::gregorian::date_facet *facet
		= new boost::gregorian::date_facet("%d-%b-%Y");
	wss.imbue(std::locale(wss.getloc(), facet));
	if (task.getDeadline() == Task::NEVER) {
		wss << L"Never";
	} else if (isPastDue(task.getDeadline())) {
		wss << L"Overdue (" << task.getDeadline().date() << L")";
	} else if (isDueAfter(task.getDeadline(), 0)) {
		wss << L"Today (" << task.getDeadline() << L")";
	} else if (isDueAfter(task.getDeadline(), 1)) {
		wss << L"Tomorrow (" << task.getDeadline().date() << L")";
	} else if (isDueAfter(task.getDeadline(), 2)) {
		wss << L"In two days (" << task.getDeadline().date() << L")";
	} else if (isDueWithin(task.getDeadline(), 7)) {
		Date dl = Date(task.getDeadline().date());
		wss << L"This coming " << dl.day_of_week()
			<< L" (" << task.getDeadline().date() << L")";
	} else if (isDueWithin(task.getDeadline(), 14)) {
		Date dl = Date(task.getDeadline().date());
		wss << L"Next " << dl.day_of_week()
			<< L" (" << task.getDeadline().date() << L")";
	} else if (isDueWithin(task.getDeadline(), 21)) {
		wss << L"Within three weeks (" << task.getDeadline().date() << L")";
	} else if (isDueWithin(task.getDeadline(), 28)) {
		wss << L"Within one month (" << task.getDeadline().date() << L")";
	} else {
		wss << L"More than a month away (" << task.getDeadline() << L")";
	}
	return boost::lexical_cast<QString>(wss.str());
}

QString MainWindow::TaskPanelManager::getIndexAsText(
	QTreeWidgetItem item) {
	return item.text(COLUMN_INDEX);
}

QString MainWindow::TaskPanelManager::getHiddenIDAsText(
	QTreeWidgetItem item) {
	return item.text(COLUMN_HIDDEN_ID);
}

QString MainWindow::TaskPanelManager::getDescriptionAsText(
	QTreeWidgetItem item) {
	return item.text(COLUMN_DESCRIPTION);
}

QString MainWindow::TaskPanelManager::getDeadlineAsText(
	QTreeWidgetItem item) {
	return item.text(COLUMN_DEADLINE);
}

QString MainWindow::TaskPanelManager::getPriorityAsText(
	QTreeWidgetItem item) {
	return item.text(COLUMN_PRIORITY);
}

QString MainWindow::TaskPanelManager::getDependenciesAsText(
	QTreeWidgetItem item) {
	return item.text(COLUMN_DEPENDENCIES);
}

QString MainWindow::TaskPanelManager::getCompletionAsText(
	QTreeWidgetItem item) {
	return item.text(COLUMN_COMPLETION);
}

}  // namespace GUI
}  // namespace You

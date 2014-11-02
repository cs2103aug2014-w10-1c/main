//@author A0094446X
#pragma once
#ifndef YOU_GUI_TASK_PANEL_MANAGER_H_
#define YOU_GUI_TASK_PANEL_MANAGER_H_
#include <QApplication>
#include "You-Controller/controller.h"
#include "You-Controller/controller_context.h"
#include "You-Controller/result.h"
#include "base_manager.h"

namespace You {
namespace GUI {

namespace UnitTests { class MainWindowTests; }

using Task = You::Controller::Task;

/// The component that deals with everything in the task panel. It handles the
/// presentation of logic of a set of tasks presented to it by the NLP engine.
/// It inherits from the BaseManager class.
class MainWindow::TaskPanelManager : public MainWindow::BaseManager{
	Q_OBJECT
	friend class UnitTests::MainWindowTests;
public:
	/// Constructor inherited from BaseManager.
	explicit TaskPanelManager(MainWindow* const parentGUI);

	/// Destructor.
	~TaskPanelManager();

	/// Initializes the taskTreePanel by setting column count and headers.
	/// Called in the constructor of MainWindow.
	void setup();

	/// Adds a task to the taskTreePanel. Only deals with top-level tasks.
	void addTask(const Task& task);

	/// Edits a task.
	void editTask(const Task& task);

	/// Deletes a task.
	void deleteTask(Task::ID taskID);


	void repaintTasks();

	/// Constructs a tree from a task and its subtasks
	std::unique_ptr<QTreeWidgetItem> makeTree(
		const Task& task, const std::map<Task::ID, Task> taskMap);

	/// Appends a child task to an existing parent task
	void addSubtask(const Task& parentTask, const Task& childTask);

	/// Time function to check if a deadline is past due.
	static bool isPastDue(Task::Time deadline);

	/// Time function to check if a deadline is due on the daysLeft'th day.
	static bool isDueAfter(Task::Time deadline, int daysLeft);

	/// Time function to check if a deadline is due within daysLeft days
	static bool isDueWithin(Task::Time deadline, int daysLeft);

	/// Time function to check if a deadline is due within exactly daysLeft days
	static bool isDueWithinExactly(Task::Time deadline, int days);

	/// Returns human-readable form for a deadline
	static QString getReadableDeadline(Task task);

	/// Colors a task
	void colorTask(QTreeWidgetItem *taskItem, QColor color, QFont font);

private:
	/// Converts the properties of a task into a set of cells for display
	/// in a QTreeWidget
	static QStringList taskToStrVec(const Task& task);

	/// Produces a generic QTreeWidgetItem from a task. It is an
	/// intermediate step to adding headings and tasks.
	static std::unique_ptr<QTreeWidgetItem> createItem(
		const You::Controller::Task& task);

	/// Produces a generic QTreeWidgetItem from an item. It is an
	/// intermediate step to adding headings and tasks.
	static std::unique_ptr<QTreeWidgetItem> createItem(
		const QStringList& cells);

	/// Finds the items which display the given task ID.
	QList<QTreeWidgetItem*> findItems(Task::ID taskID) const;

	/// Adds a subtask to the taskTreePanel. Requires the specification of a
	/// parent task.
	void addSubtask(QTreeWidgetItem* parent, const QStringList& rowStrings);

	/// Deletes a task or subtask. Memory management is automagically dealt
	/// with by QT's parent/child structure, so all child objects are
	/// automatically deleted.
	void deleteTask(QTreeWidgetItem* task);

	QScopedPointer<QMenu> itemContextMenu;

	QAction addAction;

	QAction deleteAction;

	QAction editAction;

	QSignalMapper deleteSignalMapper;

	QSignalMapper editSignalMapper;

private slots:
	void contextMenu(const QPoint &pos);

	/// Updates row numbers
	void updateRowNumbers();

private:
	/// String/numeric constants for the GUI
	/// @{
	/// Header string for column 1
	static const QString TASK_COLUMN_0_TITLE;

	/// Header string for column 2
	static const QString TASK_COLUMN_1_TITLE;

	/// Header string for column 3
	static const QString TASK_COLUMN_2_TITLE;

	/// Header string for column 4
	static const QString TASK_COLUMN_3_TITLE;

	/// Header string for column 5
	static const QString TASK_COLUMN_4_TITLE;

	/// Header string from column 6
	static const QString TASK_COLUMN_5_TITLE;

	/// Header string from column 7
	static const QString TASK_COLUMN_6_TITLE;

	/// Column reference to Index column
	static const int COLUMN_INDEX;

	/// Column reference to Hidden ID column
	static const int COLUMN_HIDDEN_ID;
	
	/// Column reference to Description column
	static const int COLUMN_DESCRIPTION;
	
	/// Column reference to Deadline column
	static const int COLUMN_DEADLINE;
	
	/// Column reference to Priority column
	static const int COLUMN_PRIORITY;
	
	/// Column reference to Dependencies column
	static const int COLUMN_DEPENDENCIES;
	
	/// Column reference to Completion column
	static const int COLUMN_COMPLETION;

public:
	QString getIndexAsText(QTreeWidgetItem item);

	QString getHiddenIDAsText(QTreeWidgetItem item);
	
	QString getDescriptionAsText(QTreeWidgetItem item);
	
	QString getDeadlineAsText(QTreeWidgetItem item);
	
	QString getPriorityAsText(QTreeWidgetItem item);
	
	QString getDependenciesAsText(QTreeWidgetItem item);
	
	QString getCompletionAsText(QTreeWidgetItem item);
	/// @}
};

}  // namespace GUI
}  // namespace You

#endif  // YOU_GUI_TASK_PANEL_MANAGER_H_

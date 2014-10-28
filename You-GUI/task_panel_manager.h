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

	/// Time function to check if a deadline is past due.
	static bool isPastDue(Task::Time deadline);

	/// Time function to check if a deadline is due on the daysLeft'th day.
	static bool isDueAfter(Task::Time deadline, int daysLeft);

	/// Time function to check if a deadline is due within daysLeft days
	static bool isDueWithin(Task::Time deadline, int daysLeft);

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

	/// Updates row numbers
	void updateRowNumbers();

	QScopedPointer<QMenu> itemContextMenu;

	QAction addAction;

	QAction deleteAction;

	QAction editAction;

	QSignalMapper deleteSignalMapper;

	QSignalMapper editSignalMapper;

private slots:
	void contextMenu(const QPoint &pos);

private:
	/// String/numeric constants for the GUI
	/// @{
	/// Header string for column 1
	static const QString TASK_COLUMN_1;

	/// Header string for column 2
	static const QString TASK_COLUMN_2;

	/// Header string for column 3
	static const QString TASK_COLUMN_3;

	/// Header string for column 4
	static const QString TASK_COLUMN_4;

	/// Header string for column 5
	static const QString TASK_COLUMN_5;

	/// Header string from column 6
	static const QString TASK_COLUMN_6;

	/// Header string from column 6
	static const QString TASK_COLUMN_7;
	/// @}
};

}  // namespace GUI
}  // namespace You

#endif  // YOU_GUI_TASK_PANEL_MANAGER_H_

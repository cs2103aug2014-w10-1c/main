//@author A0094446X
#pragma once
#ifndef YOU_GUI_MAIN_WINDOW_H_
#define YOU_GUI_MAIN_WINDOW_H_
#include <memory>
#include <QtWidgets/QMainWindow>
#include "You-Controller/result.h"
#include "ui_yougui.h"
#include "command_text_box.h"

namespace You {
namespace GUI {

namespace UnitTests { class MainWindowTests; }

using Task = You::Controller::Task;
using TaskList = You::Controller::TaskList;

/// The entity that deals with all GUI operations, and makes calls to the NLP
/// engine. It deals with basic tasks regarding GUI initialization, passes all
/// user input to the NLP engine and listens for any return instructions.
class MainWindow : public QMainWindow {
	Q_OBJECT
	friend class UnitTests::MainWindowTests;
public:
	/// Constructor for the GUI.
	explicit MainWindow(QWidget *parent = nullptr);

	/// Destructor for the GUI.
	~MainWindow();

	/// Populates the task panel with data. This is not vital to the execution
	/// of the program; it merely serves example data.
	void populateTaskPanel();

	/// The class from which all components inherit.
	class BaseManager;

	/// The component that handles all session-related data of the application.
	/// It handles basics such as the window state of the application and task
	/// sets previously displayed, saving the state on program exit, and
	/// restoring them when the program is started again. It inherits from the
	/// BaseManager class.
	class SessionManager;

	/// The component that deals with everything in the task panel. It handles
	/// the presentation of logic of a set of tasks presented to it by the NLP
	/// engine. It inherits from the BaseManager class.
	class TaskPanelManager;

	/// The component that handles all interactions with the tray icon. It
	/// handles the window state of the application based on commands sent to
	/// the tray manager. It inherits from the BaseManager class.
	class SystemTrayManager;

	/// The component that handles passing of data between the NLP parser and
	/// the rest of the GUI. It has two primary operations: on setup it passes
	/// a list containing the IDs of tasks that were previously open, and
	/// receives a Result object. Every time a command is sent, the current
	/// Result as well as input string is sent to the parser. The Result is a
	/// vector of tasks, and this replaces the existing Result. It inherits
	/// from the BaseManager class.
	class QueryManager;

	/// Calls TaskPanelManager and requests addition of a single task.
	void addTask(const Task& task);

	/// Calls TaskPanelManager and requests addition of a list of tasks.
	void addTasks(const TaskList& tl);

	/// Calls TaskPanelManager and requests editing of a single task.
	void editTask(const Task& task);

	/// Calls TaskPanelManager and requests deletion of a single task.
	void deleteTask(Task::ID taskID);

	/// Clears the task panel of all tasks, and clears taskList.
	void clearTasks();

	/// Gets the current list of tasks.
	const TaskList& getTaskList() const;

signals:
	void taskPanelContextMenu(const QPoint &pos);

protected:
	/// Reimplementation of QMainWindow's resizeEvent to try and preserve
	/// task panel proportions on resize.
	void resizeEvent(QResizeEvent* event);

private:
	/// The SessionManager instance
	const std::unique_ptr<SessionManager> sm;

	/// The TaskPanelManager instance
	const std::unique_ptr<TaskPanelManager> tpm;

	/// The SystemTrayManager instance
	const std::unique_ptr<SystemTrayManager> stm;

	/// The QueryManager instance
	const std::unique_ptr<QueryManager> qm;

private:
	/// The QT object that holds all items that are defined when building the
	/// UI in Designer. All UI objects must be referenced through this class.
	Ui::MainWindowClass ui;

	const std::unique_ptr<CommandTextBox> commandTextBox;

	/// TaskList containing tasks to be placed in the task panel
	std::unique_ptr<TaskList> taskList;

	/// Reimplementation of setVisible for system tray manager
	void setVisible(bool visible);

	/// Reimplementation of closeEvent to save state of GUI.
	void closeEvent(QCloseEvent *event);

	/// Sends the current query to the NLP manager.
	void sendQuery();

private:
	static const QString READY_MESSAGE;
	static const QString EMPTY_TASK_DESCRIPTION_MESSAGE;
	static const QString TASK_NOT_FOUND_MESSAGE;
	static const QString PARSE_ERROR_MESSAGE;
	static const QString PARSER_EXCEPTION_MESSAGE;
	static const QString RESOURCE_RED;
	static const QString RESOURCE_GREEN;
	static const QString CONTEXT_INDEX_OUT_OF_RANGE_MESSAGE;
	static const QString CONTEXT_REQUIRED_MESSAGE;
	static const QString CIRCULAR_DEPENDENCY_MESSAGE;
	static const QString NOT_UNDOABLE_MESSAGE;
	static const QString PARSER_TYPE_MESSAGE;
	static const QString UNKNOWN_EXCEPTION_MESSAGE;

private slots:
	/// Sends a query to Controller from the commandInputBox.
	void commandEnterPressed();

	/// Sends a query to Controller from the commandInputBox.
	void commandEnterButtonClicked();

	/// Reimplementation of application exit, called from SystemTrayManager
	void applicationExitRequested();

	/// Task panel context menu Add action
	void contextAddTask();

	/// Task panel context menu Edit/Delete action
	void contextDeleteTask(int id);

	void contextEditTask(int id);

	/// Updates task descriptor panel on task selection.
	void taskSelected();

	Task::ID getSelectedTaskID();
};


}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_MAIN_WINDOW_H_

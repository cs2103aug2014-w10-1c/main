//@author A0094446X
#pragma once
#ifndef YOU_GUI_MAIN_WINDOW_H_
#define YOU_GUI_MAIN_WINDOW_H_
#include <memory>
#include <QtWidgets/QMainWindow>
#include "ui_yougui.h"
#include "You-Controller/result.h"

namespace You {
namespace GUI {
namespace UnitTests { class MainWindowTests; }
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

	void addTask(const You::Controller::Task& task);

	void addTasks(const You::Controller::TaskList& tl);

	void editTask(const You::Controller::Task& task);

	void deleteTask(You::Controller::Task::ID taskID);

	const You::Controller::TaskList& getTaskList() const;

protected:
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

	std::unique_ptr<You::Controller::TaskList> taskList;
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

private slots:
	/// Qt's slot for hitting enter in the input box.
	void commandEnterPressed();

	/// Qt's signal/slot mechanism for input enter button.
	void commandEnterButtonClicked();

	void applicationExitRequested();
};

}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_MAIN_WINDOW_H_

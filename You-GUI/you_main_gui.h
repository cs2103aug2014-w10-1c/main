//@author A0094446X
#pragma once
#ifndef YOU_GUI_YOU_MAIN_GUI_H_
#define YOU_GUI_YOU_MAIN_GUI_H_
#include <memory>
#include <QtWidgets/QMainWindow>
#include "ui_yougui.h"
#include "You-NLP/task_list.h"
/// The entity that deals with all GUI operations, and makes calls to the NLP
/// engine. It deals with basic tasks regarding GUI initialization, passes all
/// user input to the NLP engine and listens for any return instructions.
class YouMainGUI : public QMainWindow {
	Q_OBJECT

public:
	/// Constructor for the GUI.
	explicit YouMainGUI(QWidget *parent = nullptr);

	/// Destructor for the GUI.
	~YouMainGUI();

	/// String/numeric constants for the GUI
	/// Number of columns in task panel
	int TASK_COLUMN_COUNT = 4;

	/// Header string for column 1
	const std::wstring TASK_COLUMN_1 = L"Hidden ID Column";

	/// Header string for column 2
	const std::wstring TASK_COLUMN_2 = L"Index";

	/// Header string for column 3
	const std::wstring TASK_COLUMN_3 = L"Description";

	/// Header string for column 4
	const std::wstring TASK_COLUMN_4 = L"Deadline";

	/// Header string for column 5
	const std::wstring TASK_COLUMN_5 = L"Priority";

	/// Vector of strings for the headers of the columns
	std::vector<std::wstring> columnHeaders;

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

	/// The component that handles all interactions with the tray icon. It handles
	/// the window state of the application based on commands sent to the tray
	/// manager. It inherits from the BaseManager class.
	class SystemTrayManager;

	/// The component that handles passing of data between the NLP parser and
	/// the rest of the GUI. It has two primary operations: on setup it passes
	/// a list containing the IDs of tasks that were previously open, and
	/// receives a Result object. Every time a command is sent, the current
	/// Result as well as input string is sent to the parser. The Result is a
	/// vector of tasks, and this replaces the existing Result. It inherits
	/// from the BaseManager class.
	class NLPManager;

	/// The SessionManager instance
	const std::unique_ptr<SessionManager> sm;

	/// The TaskPanelManager instance
	const std::unique_ptr<TaskPanelManager> tpm;

	/// The SystemTrayManager instance
	const std::unique_ptr<SystemTrayManager> stm;

	/// The NLPManager instance
	const std::unique_ptr<NLPManager> nlpm;

private:
	/// The QT object that holds all items that are defined when building the
	/// UI in Designer. All UI objects must be referenced through this class.
	Ui::YouMainGUIClass ui;

	std::unique_ptr<You::NLP::TaskList> taskList;

	/// Reimplementation of setVisible for system tray manager
	void setVisible(bool visible);

	/// Reimplementation of closeEvent to save state of GUI.
	void closeEvent(QCloseEvent *event);
};

#endif  // YOU_GUI_YOU_MAIN_GUI_H_

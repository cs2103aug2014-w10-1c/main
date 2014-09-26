#pragma once
#ifndef YOU_GUI_YOU_MAIN_GUI_H_
#define YOU_GUI_YOU_MAIN_GUI_H_

#include <QtWidgets/QMainWindow>
#include <QList>
#include "../You-NLP/controller.h"
#include "../You-NLP/result.h"
#include "ui_yougui.h"

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
	const int TASK_COLUMN_COUNT = 4;
	std::vector<std::wstring> columnHeaders;
	/// Header string for column 1
	const std::wstring TASK_COLUMN_1 = L"Hidden ID Col";

	/// Header string for column 2
	const std::wstring TASK_COLUMN_2 = L"Index";

	/// Header string for column 3
	const std::wstring TASK_COLUMN_3 = L"Task Title";

	/// Header string for column 4
	const std::wstring TASK_COLUMN_4 = L"Task Description";

	/// Header string for column 5
	const std::wstring TASK_COLUMN_5 = L"Due Date";

	/// Populates the task panel with data. This is not vital to the execution
	/// of the program; it merely serves example data.
	void populateTaskPanel();

private:

	/// The QT object that holds all items that are defined when building the
	/// UI in Designer. All UI objects must be referenced through this class.
	Ui::YouMainGUIClass ui;

	class YouMainGUI::SessionManager {
		friend class YouMainGUI;
	public:
		YouMainGUI & parentGUI;
		SessionManager(YouMainGUI & parentGUI);
		~SessionManager();
	private:
		/// Loads the previous state of the GUI. Called during constructor.
		void loadSession();

		/// Saves the state of the GUI before closing. Called during closeEvent.
		void saveSession();
	};
	/// Local instance of a session manager.
	SessionManager sm;
	/// Reimplementation of closeEvent to save state of GUI.
	void closeEvent(QCloseEvent *event);

	/// All user action functions. Probably will all be of
	/// on_(ui_element)_(event) type. Depending on the function,
	/// it will convert current program state into a context,
	/// populate commandInputBox, call queryNLP() to parse that command,
	/// then clear the commandInputBox and do updateTreeWidget();
	/// Queries the NLP engine. Converts the current view into a context,
	/// passes the context and input into the NLP engine, and gets a
	/// Result object. Called by user's confirmation to send entry in
	/// commandInputBox. This is currently just a placeholder.
	You::NLP::Result queryNLP();


	class TaskPanelManager {
		friend class YouMainGUI;
	public:
		YouMainGUI & parentGUI;
		TaskPanelManager(YouMainGUI & parentGUI);
		~TaskPanelManager();
	private:
		/// Updates tree widget as the result of a query. This is
		/// currently just a placeholder.
		void updateTreeWidget(You::NLP::Result result);

		/// Initializes the taskTreePanel by setting column count and headers.
		void taskPanelSetup();

		/// Produces a generic QTreeWidgetItem from a vector of wstrings. It is an
		/// intermediate step to adding headings and tasks.
		QTreeWidgetItem* createItem(std::vector<std::wstring> rowStrings);

		/// Adds a task to the taskTreePanel. Only deals with top-level tasks.
		void addTask(std::vector<std::wstring> rowStrings);

		/// Adds a subtask to the taskTreePanel. Requires the specification of a
		/// parent task.
		void addSubtask(QTreeWidgetItem* parent,
			std::vector<std::wstring> rowStrings);

		/// Deletes a task or subtask. Memory management is automagically dealt
		/// with by QT's parent/child structure, so all child objects are
		/// automatically deleted.
		void deleteTask(QTreeWidgetItem* task);
	};

	TaskPanelManager tpm;

	class SystemTrayManager {
		friend class YouMainGUI;
	public:
		YouMainGUI & parentGUI;
		SystemTrayManager(YouMainGUI & parentGUI);
		~SystemTrayManager();
	private:
		/// System Tray functions
		/// Defines and sets the tray icon. Called in the constructor.
		void setIcon();

		/// System tray icon object that adds an icon to the tray.
		QSystemTrayIcon trayIcon;

		/// QT's signal/slot mechanism for tray icon activation.
		void iconActivated(QSystemTrayIcon::ActivationReason reason);

		/// Icon image for system tray.
		QIcon icon;
	};
	SystemTrayManager stm;

	private slots:
	/// QT's signal/slot mechanism for input enter button.
	void on_commandEnterButton_clicked();
};


#endif  // YOU_GUI_YOU_MAIN_GUI_H_

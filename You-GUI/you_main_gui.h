#pragma once
#ifndef YOU_GUI_YOU_MAIN_GUI_H_
#define YOU_GUI_YOU_MAIN_GUI_H_
#include <memory>
#include <array>
#include <QtWidgets/QMainWindow>
#include <QList>
#include "You-NLP/controller.h"
#include "You-NLP/result.h"
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
	int TASK_COLUMN_COUNT = 4;
	/// Header string for column 1
	const std::wstring TASK_COLUMN_1 = L"Hidden ID Column";

	/// Header string for column 2
	const std::wstring TASK_COLUMN_2 = L"Index";

	/// Header string for column 3
	const std::wstring TASK_COLUMN_3 = L"Task Title";

	/// Header string for column 4
	const std::wstring TASK_COLUMN_4 = L"Task Description";

	/// Header string for column 5
	const std::wstring TASK_COLUMN_5 = L"Due Date";
	std::vector<std::wstring> columnHeaders;
	/// Populates the task panel with data. This is not vital to the execution
	/// of the program; it merely serves example data.
	void populateTaskPanel();

private:
	class BaseManager;
	class SessionManager;
	class TaskPanelManager;
	class SystemTrayManager;

	/// The QT object that holds all items that are defined when building the
	/// UI in Designer. All UI objects must be referenced through this class.
	Ui::YouMainGUIClass ui;
	std::unique_ptr<SessionManager> sm;
	std::unique_ptr<TaskPanelManager> tpm;
	std::unique_ptr<SystemTrayManager> stm;
	void setVisible(bool visible);
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

	private slots:
	/// QT's signal/slot mechanism for input enter button.
	void on_commandEnterButton_clicked();
};

class YouMainGUI::BaseManager : public QObject {
	Q_OBJECT
public:
	YouMainGUI * const parentGUI;
	BaseManager();
	explicit BaseManager(YouMainGUI * parentGUI);
	void setup();
};


#endif  // YOU_GUI_YOU_MAIN_GUI_H_

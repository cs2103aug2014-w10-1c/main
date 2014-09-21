#pragma once
#ifndef YOU_MAIN_GUI_YOUGUI_H_
#define YOU_MAIN_GUI_YOUGUI_H_

#include <QtWidgets/QMainWindow>
#include <QList>
#include "ui_yougui.h"

/// The entity that deals with all GUI operations, and makes calls to the NLP engine. It deals with
/// basic tasks regarding GUI initialization, passes all user input to the NLP engine and listens
/// for any return instructions.
class YouMainGUI : public QMainWindow {
	Q_OBJECT

public:
	/// Constructor for the GUI.
	explicit YouMainGUI(QWidget *parent = nullptr);
	
	/// Destructor for the GUI.
	~YouMainGUI();

	/// Populates the task panel with data. This is not vital to the execution of the program; it
	/// merely serves example data.
	void populateTaskPanel();
private:

	/// Initializes the taskTreePanel by setting column count and headers.
	void taskPanelSetup();

	/// Produces a generic QTreeWidgetItem from a vector of wstrings. It is an intermediate step to 
	/// adding headings and tasks.
	QTreeWidgetItem* createItem(std::vector<std::wstring> rowStrings);

	/// Adds a task to the taskTreePanel. Only deals with top-level tasks.
	void addTask(std::vector<std::wstring> rowStrings);

	/// Adds a subtask to the taskTreePanel. Requires the specification of a parent task.
	void addSubtask(QTreeWidgetItem* parent, std::vector<std::wstring> rowStrings);

	/// Deletes a task or subtask. Memory management is automagically dealt with by QT's parent/child
	/// structure, so all child objects are automatically deleted.
	void deleteTask(QTreeWidgetItem* task);

	/// The QT object that holds all items that are defined when building the UI in Designer.
	/// All UI objects must be referenced through this class.
	Ui::YouMainGUIClass ui;

	/// Manages the state of the GUI on loading and saving for persistence.
	void loadSession();
	void saveSession();
	void closeEvent(QCloseEvent *event);

	private slots:
	void on_commandEnterButton_clicked();
};

#endif  // YOU_MAIN_GUI_YOUGUI_H_

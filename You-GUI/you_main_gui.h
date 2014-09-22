#pragma once
#ifndef YOU_GUI_YOU_MAIN_GUI_H_
#define YOU_GUI_YOU_MAIN_GUI_H_

#include <QtWidgets/QMainWindow>
#include <QList>
#include "ui_yougui.h"
#include "..\You-NLP\controller.h"
#include "..\You-NLP\result.h"

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

	/// String/numeric constants the GUI
	const int TASK_COLUMN_COUNT = 4;
	const std::wstring TASK_COLUMN_1 = L"Hidden ID Col";
	const std::wstring TASK_COLUMN_2 = L"Index";
	const std::wstring TASK_COLUMN_3 = L"Task Title";
	const std::wstring TASK_COLUMN_4 = L"Task Description";
	const std::wstring TASK_COLUMN_5 = L"Due Date";

	/// Populates the task panel with data. This is not vital to the execution
	/// of the program; it merely serves example data.
	void populateTaskPanel();

private:
	/// Creates the NLP parser
	You::NLP::Controller controller;

	/// Queries the NLP engine. Converts the current view into a context,
	/// passes the context and input into the NLP engine, and gets a
	/// Result object. This is currently just a placeholder.
	void queryNLP();

	/// Updates tree widget as the result of a query. This is currently just a placeholder.
	void updateTreeWidget();

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

	/// The QT object that holds all items that are defined when building the
	/// UI in Designer. All UI objects must be referenced through this class.
	Ui::YouMainGUIClass ui;

	/// Loads the previous state of the GUI. Called during constructor.
	void loadSession();

	/// Saves the state of the GUI before closing. Called during closeEvent
	void saveSession();

	/// Reimplementation of closeEvent to save state of GUI
	void closeEvent(QCloseEvent *event);

	private slots:
	void on_commandEnterButton_clicked();
};

#endif  // YOU_GUI_YOU_MAIN_GUI_H_

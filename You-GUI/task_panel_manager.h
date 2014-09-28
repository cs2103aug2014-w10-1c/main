#pragma once
#ifndef YOU_GUI_TASK_PANEL_MANAGER_H_
#define YOU_GUI_TASK_PANEL_MANAGER_H_
#include <QApplication>
#include "base_manager.h"

/// The component that deals with everything in the task panel. It handles the
/// presentation of logic of a set of tasks presented to it by the NLP engine.
/// It inherits from the BaseManager class.
class YouMainGUI::TaskPanelManager : public YouMainGUI::BaseManager{
	Q_OBJECT
	friend class YouMainGUI;
public:
	explicit TaskPanelManager(YouMainGUI * const parentGUI)
		: BaseManager(parentGUI) {}
	~TaskPanelManager();

private:
	/// Initializes the taskTreePanel by setting column count and headers.
	/// Called in the constructor of YouMainGUI.
	void setup();
	/// Updates tree widget as the result of a query. This is currently just a
	/// placeholder.
	void updateTreeWidget(You::NLP::Result result);

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

#endif  // YOU_GUI_TASK_PANEL_MANAGER_H_

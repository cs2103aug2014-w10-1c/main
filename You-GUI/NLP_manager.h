//@author A0094446X
#pragma once
#ifndef YOU_GUI_NLP_MANAGER_H_
#define YOU_GUI_NLP_MANAGER_H_
#include <QApplication>
#include <QWidget>
#include "You-Controller/controller.h"
#include "You-Controller/result.h"
#include "You-Controller/controller_context.h"
#include "base_manager.h"

/// The component that handles passing of data between the NLP parser and the
/// rest of the GUI. It has two primary operations: on setup it passes a list
/// containing the IDs of tasks that were previously open, and receives a
/// Result object. Every time a command is sent, the current Result as well
/// as input string is sent to the parser. The Result is a vector of tasks, and
/// this replaces the existing Result. It inherits from the BaseManager class.
class YouMainGUI::NLPManager : public YouMainGUI::BaseManager{
public:
	/// Constructor inherited from BaseManager.
	explicit NLPManager(YouMainGUI* const parentGUI)
		: BaseManager(parentGUI) {}

	/// Wrapper function to connect commandEnterButton to NLP call signal/slot
	void setup();

	/// Queries the NLP engine. Sends the query into the NLP engine, and gets a
	/// Result which it will process.
	void query(const QString& query, const You::Controller::TaskList& taskList);

	/// Gets the tasks with the given task IDs.
	You::Controller::TaskList getTasks(
		const QList<You::Controller::Task::ID>& taskIDs);

	/// Gets all tasks.
	You::Controller::TaskList getTasks();

private:
	/// The current result/context displayed to the user
	std::unique_ptr<You::Controller::Result> currentResult;
};

#endif  // YOU_GUI_NLP_MANAGER_H_

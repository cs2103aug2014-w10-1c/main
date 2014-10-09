//@author A0094446X
#include "stdafx.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <algorithm>
#include <QApplication>
#include <QList>
#include "You-Controller\exception.h"
#include "NLP_manager.h"

using Task = You::Controller::Task;
using Result = You::Controller::Result;
using TaskList = You::Controller::TaskList;
using Controller = You::Controller::Controller;

void YouMainGUI::NLPManager::setup() {
	// To change to get list of tasks from session instead of ALL tasks.
	// Somehow pass sm's taskIDs into this.
	// Set result to current context
}

void YouMainGUI::NLPManager::query(
	const QString& query,
	const You::Controller::TaskList& taskList) {

	Result result = Controller::get().query(query.toStdWString(), taskList);

	struct ResultProcessorVisitor : boost::static_visitor<void> {
		explicit ResultProcessorVisitor(YouMainGUI* const parentGUI)
			: parentGUI(parentGUI) {
		}

		void operator()(You::Controller::ADD_RESULT addResult) {
			parentGUI->addTask(addResult.task);
		}
		void operator()(You::Controller::EDIT_RESULT editResult) {
			parentGUI->editTask(editResult.task);
		}
		void operator()(You::Controller::DELETE_RESULT deleteResult) {
			parentGUI->deleteTask(deleteResult.task);
		}

	private:
		YouMainGUI* parentGUI;
	};

	ResultProcessorVisitor visitor(parentGUI);
	boost::apply_visitor(visitor, result);
	
}

TaskList YouMainGUI::NLPManager::getTasks(
	const QList<Task::ID>& taskIDs) {
	std::vector<Task::ID> taskIDVector;
	std::copy(
		taskIDs.begin(),
		taskIDs.end(),
		std::back_inserter(taskIDVector)
	);

	return Controller::get().getTasks(taskIDVector);
}

TaskList YouMainGUI::NLPManager::getTasks() {
	return Controller::get().getTasks();
}

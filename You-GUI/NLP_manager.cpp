//@author A0094446X
#include "stdafx.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QApplication>
#include <QList>
#include "NLP_manager.h"
#include "You-QueryEngine/internal/task_builder.h"

using Task = You::Controller::Task;
using Result = You::Controller::Result;
using TaskList = You::Controller::TaskList;
using Controller = You::Controller::Controller;

void YouMainGUI::NLPManager::setup() {
	connect(parentGUI->ui.commandEnterButton,
		SIGNAL(clicked()), this, SLOT(commandEnterButtonClicked()));
	// To change to get list of tasks from session instead of ALL tasks.
	// Somehow pass sm's taskIDs into this.
	// Set result to current context
}

void YouMainGUI::NLPManager::queryNLP() {
	/// Feed query and context into NLP engine
	std::wstring inputString =
		parentGUI->ui.commandInputBox->text().toStdWString();
	// When ready, pass current context, not the default one.
	TaskList tl = parentGUI->getTaskList();
	/*
	TaskList tl;
	Task newTask =
		You::QueryEngine::Internal::TaskBuilder::get().description(L"LOL");
	tl.push_back(newTask);
	*/
	Controller::Context ct = Controller::Context::Context(tl);
	Result result = Controller::get().query(inputString, ct);

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

void YouMainGUI::NLPManager::commandEnterButtonClicked() {
	YouMainGUI::NLPManager::queryNLP();
	// For demonstration purposes
	// parentGUI->populateTaskPanel();
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

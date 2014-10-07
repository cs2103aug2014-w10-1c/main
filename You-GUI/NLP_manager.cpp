//@author A0094446X
#include "stdafx.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QApplication>
#include <QList>
#include "NLP_manager.h"
#include "variant_handler.h"
#include "You-QueryEngine/internal/task_builder.h"

using Task = You::Controller::Task;
using Result = You::Controller::Result;
using TaskList = You::Controller::TaskList;
using Controller = You::Controller::Controller;

YouMainGUI::NLPManager::~NLPManager() {
}

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
	boost::apply_visitor(*(parentGUI->vh), result);
}

void YouMainGUI::NLPManager::commandEnterButtonClicked() {
	YouMainGUI::NLPManager::queryNLP();
	// For demonstration purposes
	// parentGUI->populateTaskPanel();
}

//@author A0094446X
#include "stdafx.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QApplication>
#include <QList>
#include "NLP_manager.h"

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

You::Controller::Result YouMainGUI::NLPManager::queryNLP() {
	/// Feed query and context into NLP engine
	std::wstring inputString =
		parentGUI->ui.commandInputBox->text().toStdWString();
	// When ready, pass current context, not the default one.
	You::Controller::Result result = Controller::get().query(inputString,
		Controller::Context::Context(parentGUI->getTaskList()));
	return result;
}

void YouMainGUI::NLPManager::commandEnterButtonClicked() {
	YouMainGUI::NLPManager::queryNLP();
	// For demonstration purposes
	// parentGUI->populateTaskPanel();
}

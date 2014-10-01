//@author A0094446X
#include "stdafx.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <QApplication>
#include <QList>
#include "NLP_manager.h"

YouMainGUI::NLPManager::~NLPManager() {
}

void YouMainGUI::NLPManager::setup() {
    connect(parentGUI->ui.commandEnterButton,
		SIGNAL(clicked()), this, SLOT(commandEnterButtonClicked()));
	// To change to get list of tasks from session instead of ALL tasks.
	You::NLP::Controller::get().getTasks();
}

void YouMainGUI::NLPManager::loadTasks(std::vector<int64_t> taskID) {
	//You::NLP::Controller::get().getTasks(taskID);
	qDebug("lol");
}

You::NLP::Result YouMainGUI::NLPManager::queryNLP() {
	/// Feed query and context into NLP engine
	std::wstring inputString =
		parentGUI->ui.commandInputBox->text().toStdWString();
	You::NLP::Result result = You::NLP::Controller::get().query(inputString,
		You::NLP::Controller::Context::DEFAULT);
	return result;
}

void YouMainGUI::NLPManager::commandEnterButtonClicked() {
	//YouMainGUI::NLPManager::queryNLP();
	parentGUI->populateTaskPanel();
}

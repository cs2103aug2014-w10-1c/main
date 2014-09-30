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
	// To fix
	// You::NLP::Controller::get().query(L"",
	// You::NLP::Controller::Context::DEFAULT);
}

You::NLP::Result YouMainGUI::NLPManager::queryNLP() {
	/// Feed query and context into NLP engine
	// To fix
	// std::wstring inputString =
	// parentGUI->ui.commandInputBox->text().toStdWString();
	You::NLP::Result result;
	return result;
}

void YouMainGUI::NLPManager::commandEnterButtonClicked() {
	YouMainGUI::NLPManager::queryNLP();
}

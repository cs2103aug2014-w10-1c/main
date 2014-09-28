#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "NLP_manager.h"

YouMainGUI::NLPManager::~NLPManager() {
}

void YouMainGUI::NLPManager::setup() {
	connect(parentGUI->ui.commandEnterButton,
		SIGNAL(parentGUI->ui.commandEnterButton->clicked(false)),
		this, SLOT(commandEnterButtonClicked()));
}

You::NLP::Result YouMainGUI::NLPManager::queryNLP() {
	/// Init a task list
	/// Convert GUI state into a context
	/// Feed query and context into NLP engine
	You::NLP::TaskList tl;
	std::wstring inputString =
		parentGUI->ui.commandInputBox->text().toStdWString();
	//You::NLP::Result result = You::NLP::Controller::get().query(inputString, );
	You::NLP::Result result;
	qDebug("Reached queryNLP end");
	return result;
}

void YouMainGUI::NLPManager::commandEnterButtonClicked() {
	YouMainGUI::NLPManager::queryNLP();
}

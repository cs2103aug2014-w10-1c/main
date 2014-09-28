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
}

You::NLP::Result YouMainGUI::NLPManager::queryNLP() {
	/// Init a task list
	/// Convert GUI state into a context
	/// Feed query and context into NLP engine
	You::NLP::TaskList tl = generateTaskList(parentGUI->ui.taskTreePanel);
	std::wstring inputString =
		parentGUI->ui.commandInputBox->text().toStdWString();
	You::NLP::Result result;
	return result;
}

void YouMainGUI::NLPManager::commandEnterButtonClicked() {
	YouMainGUI::NLPManager::queryNLP();
}

You::NLP::TaskList YouMainGUI::NLPManager::generateTaskList(
		QTreeWidget* widget) {
	/// Loop through widget, building tasks and adding one by one to TaskList
	You::NLP::TaskList taskList;
	QTreeWidgetItemIterator it(widget, QTreeWidgetItemIterator::All);
	while (*it) {
		/// convert from QTreeWidgetItem to Task
		/* To Do on API clarification
		int64_t id;
		std::wstring description;
		boost::posix_time::ptime time;
		std::vector<Task::ID> Dependencies;
		enum class Priority { IMPORTANT, NORMAL };
		qDebug(QString::fromStdWString((*it)->text(1).toStdWString()).toLatin1());
		*/
		++it;
	}
	return taskList;
}

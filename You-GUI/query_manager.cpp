//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "query_manager.h"

namespace You {
namespace GUI {

using Controller = You::Controller::Controller;

void MainWindow::QueryManager::setup() {
	QPixmap pixmap;
	pixmap.fill(Qt::transparent);
	pixmap.load(":/Status_green.png", 0);
	parentGUI->ui.statusIcon->setPixmap(QPixmap(":/Status_green.png"));
	parentGUI->ui.statusIcon->show();
	parentGUI->statusBar()->insertPermanentWidget(
		0, parentGUI->ui.statusIcon, 0);
	parentGUI->statusBar()->insertPermanentWidget(
		0, parentGUI->ui.statusMessage, 0);
}

void MainWindow::QueryManager::query(
	const QString& query,
	const You::Controller::TaskList& taskList) {
	Result result = Controller::get().query(query.toStdWString(), taskList);

	struct ResultProcessorVisitor : boost::static_visitor<void> {
		explicit ResultProcessorVisitor(MainWindow* const parentGUI)
		: parentGUI(parentGUI) {
		}
		void operator()(You::Controller::ADD_RESULT addResult) {
			parentGUI->addTasks({ addResult.task });
		}
		void operator()(const You::Controller::SHOW_RESULT& showResult) {
			parentGUI->clearTasks();
			parentGUI->addTasks(showResult.tasks);
		}
		void operator()(You::Controller::EDIT_RESULT editResult) {
			parentGUI->editTask(editResult.task);
		}
		void operator()(You::Controller::DELETE_RESULT deleteResult) {
			parentGUI->deleteTask(deleteResult.task);
		}
		void operator()(const You::Controller::UNDO_RESULT& undoResult) {
			parentGUI->clearTasks();
			parentGUI->addTasks(undoResult.tasks);
		}

	private:
		MainWindow* parentGUI;
	};

	ResultProcessorVisitor visitor(parentGUI);
	boost::apply_visitor(visitor, result);
}

TaskList MainWindow::QueryManager::getTasks(
	const QList<Task::ID>& taskIDs) {
	std::vector<Task::ID> taskIDVector;
	std::copy(
		taskIDs.begin(),
		taskIDs.end(),
		std::back_inserter(taskIDVector)
	);

	return Controller::get().getTasks(taskIDVector);
}

TaskList MainWindow::QueryManager::getTasks() {
	return Controller::get().getTasks();
}

}  // namespace GUI
}  // namespace You

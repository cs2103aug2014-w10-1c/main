//@author A0094446X
#include "stdafx.h"
#include "CppUnitTest.h"
#include "You-GUI\main_window.h"
#include "You-GUI\system_tray_manager.h"
#include "You-QueryEngine\api.h"
#include "You-GUI\task_panel_manager.h"
using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Task = You::Controller::Task;
using TaskList = You::Controller::TaskList;
using Date = boost::gregorian::date;
namespace You {
namespace GUI {
namespace UnitTests {
QApplication *app;

// Simulate running the main() function
// Qt event loop
TEST_MODULE_INITIALIZE(ModuleInitialize) {
	int argc = 1;
	char *argv[] = { "You.exe" };
	app = new QApplication(argc, argv);
}

// Cleans up what we set up
TEST_MODULE_CLEANUP(ModuleCleanup) {
	app->quit();
	delete app;
}

TEST_CLASS(MainWindowTests) {
public:
	MainWindow* w;
	TEST_METHOD_INITIALIZE(MethodInitialize) {
		w = new MainWindow();
		w->clearTasks();
	}

	TEST_METHOD_CLEANUP(MethodCleanup) {
		while (w->ui.taskTreePanel->topLevelItemCount() != 0) {
			w->commandTextBox->setPlainText(QString("/delete 1"));
			w->commandEnterPressed();
		}
		w->clearTasks();
		delete w;
	}

	/// These are generic tests for component visibility/invisibility
	TEST_METHOD(visibilityTest) {
		bool visibilityTest =
			w->isVisible() &&
			w->ui.centralWidget->isVisible() &&
			w->ui.taskTreePanel->isVisible() &&
			w->ui.commandEnterButton->isVisible() &&
			w->ui.statusBar->isVisible() &&
			w->ui.statusIcon->isVisible() &&
			w->ui.statusMessage->isVisible() &&
			!w->ui.mainToolBar->isVisible() &&
			!w->ui.menuBar->isVisible();
		Assert::IsTrue(visibilityTest);
	}

	/// Basic task addition test
	TEST_METHOD(addSingleTask) {
		w->clearTasks();
		w->commandTextBox->setPlainText(QString("/add test by Nov 99"));
		w->commandEnterPressed();

		QTreeWidgetItem item = *w->ui.taskTreePanel->topLevelItem(0);
		int column1 = QString::compare(item.text(0), QString("1"));
		int column2 = QString::compare(item.text(2), QString("test"));
		int column3 = QString::compare(
			item.text(4),
			QString("Overdue (01 November 1999)"));
		int column4 = QString::compare(item.text(5), QString("Normal"));

		Assert::IsTrue(w->ui.taskTreePanel->topLevelItemCount() == 1 &&
			(column1 == 0) && (column2 == 0) &&
			(column3 == 0) && (column4 == 0));
	}

	TEST_METHOD(editSingleTask) {
		w->clearTasks();
		w->commandTextBox->setPlainText(QString("/add test by Nov 99"));
		w->commandEnterPressed();
		w->commandTextBox->setPlainText(
			QString("/edit 1 set description = 'abc'"));
		w->commandEnterPressed();
		QTreeWidgetItem item = *w->ui.taskTreePanel->topLevelItem(0);
		int column1 = QString::compare(item.text(0), QString("1"));
		int column2 = QString::compare(item.text(2), QString("abc"));
		int column3 = QString::compare(
			item.text(4),
			QString("Overdue (01 November 1999)"));
		int column4 = QString::compare(item.text(5), QString("Normal"));
		int i = w->ui.taskTreePanel->topLevelItemCount();
		w->clearTasks();
		Assert::IsTrue(i == 1 &&
			(column1 == 0) && (column2 == 0) &&
			(column3 == 0) && (column4 == 0));
	}

	/// Boundary test case for the equivalence partition for the lower
	/// bound of the valid zone for testing if a task is due today
	TEST_METHOD(testDueToday1) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		Assert::IsTrue(MainWindow::TaskPanelManager::isDueAfter(dl, 0));
	}

	/// Generic test case for testing if a task is due today (deadline ahead)
	TEST_METHOD(testDueToday2) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl += boost::posix_time::hours(24) + boost::posix_time::minutes(1);
		Assert::IsFalse(MainWindow::TaskPanelManager::isDueAfter(dl, 0));
	}

	/// Generic test case for testing if a task is due today (deadline before)
	TEST_METHOD(testDueToday3) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl -= (boost::posix_time::hours(24) + boost::posix_time::minutes(1));
		Assert::IsFalse(MainWindow::TaskPanelManager::isDueAfter(dl, 0));
	}

	/// Generic test case for testing if a task is due
	/// tomorrow (deadline after)
	TEST_METHOD(testDueTomorrow1) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl += boost::posix_time::hours(24);
		Assert::IsTrue(MainWindow::TaskPanelManager::isDueAfter(dl, 1));
	}

	/// Boundary test case for the equivalence partition for the upper
	/// bound of the lower invalid zone for testing if a task is overdue
	TEST_METHOD(testPastDue1) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl -= boost::posix_time::minutes(1);
		Assert::IsTrue(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Generic test case for testing if a task is overdue
	TEST_METHOD(testPastDue2) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl -= (boost::posix_time::hours(24) + boost::posix_time::minutes(1));
		Assert::IsTrue(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Test if is past due, on the same time
	TEST_METHOD(testPastDue3) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		Assert::IsFalse(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Test if is past due, with deadline 1 minute after
	TEST_METHOD(testPastDue4) {
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl += boost::posix_time::minutes(1);
		Assert::IsFalse(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Generic task deletion test
	TEST_METHOD(deleteSingleTaskCount) {
		w->clearTasks();
		w->commandTextBox->setPlainText(QString("/add test by Nov 20"));
		w->commandEnterPressed();
		w->commandTextBox->setPlainText(QString("/add test2 by Nov 20"));
		w->commandEnterPressed();
		w->commandTextBox->setPlainText(QString("/delete 1"));
		w->commandEnterPressed();
		Assert::IsTrue(w->ui.taskTreePanel->topLevelItemCount() == 1);
	}

	/// Generic task deletion test
	TEST_METHOD(deleteSingleTaskFind) {
		w->clearTasks();
		w->commandTextBox->setPlainText(QString("/add test by Nov 20"));
		w->commandEnterPressed();
		w->commandTextBox->setPlainText(QString("/add test2 by Nov 20"));
		w->commandEnterPressed();
		w->commandTextBox->setPlainText(QString("/delete 1"));
		w->commandEnterPressed();
		Assert::IsTrue(w->ui.taskTreePanel->findItems(
			QString("1"), Qt::MatchExactly, 1).size() == 0);
	}
};
}  // namespace UnitTests
}  // namespace GUI
}  // namespace You

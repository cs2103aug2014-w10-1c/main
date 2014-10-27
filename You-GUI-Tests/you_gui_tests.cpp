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
// Sets up the logging facility and the Qt event loop

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
	/// These are generic tests for component visibility/invisibility
	TEST_METHOD(isMainWindowVisible) {
		MainWindow w;
		Assert::IsTrue(w.isVisible());
	}

	TEST_METHOD(isCentralWidgetVisible) {
		MainWindow w;
		Assert::IsTrue(w.ui.centralWidget->isVisible());
	}

	TEST_METHOD(isTaskPanelVisible) {
		MainWindow w;
		Assert::IsTrue(w.ui.taskTreePanel->isVisible());
	}

	TEST_METHOD(isCommandEnterButtonVisible) {
		MainWindow w;
		Assert::IsTrue(w.ui.commandEnterButton->isVisible());
	}

	TEST_METHOD(isCommandTextBoxVisible) {
		MainWindow w;
		Assert::IsTrue(w.commandTextBox->isVisible());
	}

	TEST_METHOD(isStatusBarVisible) {
		MainWindow w;
		Assert::IsTrue(w.ui.statusBar->isVisible());
	}

	TEST_METHOD(isStatusIconVisible) {
		MainWindow w;
		Assert::IsTrue(w.ui.statusIcon->isVisible());
	}

	TEST_METHOD(isStatusMessageVisible) {
		MainWindow w;
		Assert::IsTrue(w.ui.statusMessage->isVisible());
	}

	TEST_METHOD(isMainToolBarHidden) {
		MainWindow w;
		Assert::IsFalse(w.ui.mainToolBar->isVisible());
	}

	TEST_METHOD(isMenuBarHidden) {
		MainWindow w;
		Assert::IsFalse(w.ui.menuBar->isVisible());
	}

	/// Basic task addition test
	TEST_METHOD(addSingleTaskCount) {
		MainWindow w;
		w.clearTasks();
		w.commandTextBox->setPlainText(QString("/add test by Nov 20"));
		w.commandEnterPressed();
		Assert::IsTrue(w.ui.taskTreePanel->topLevelItemCount() == 1);
	}

	/// Task addition content comparison
	TEST_METHOD(addSingleTaskContent) {
		MainWindow w;
		w.clearTasks();
		w.commandTextBox->setPlainText(QString("/add test by Nov 2099"));
		w.commandEnterPressed();
		QTreeWidgetItem item = *w.ui.taskTreePanel->topLevelItem(0);
		int column1 = QString::compare(item.text(1), QString("0"));
		int column2 = QString::compare(item.text(2), QString("test"));
		int column3 = QString::compare(
			item.text(3),
			QString("More than a month away (2099-Nov-01 00:00:00)"));
		int column4 = QString::compare(item.text(4), QString("Normal"));
		Assert::IsTrue((column1 == 0) && (column2 == 0) &&
			(column3 == 0) && (column4 == 0));
	}

	/// Boundary test case for the equivalence partition for the lower
	/// bound of the valid zone for testing if a task is due today
	TEST_METHOD(testDueToday1) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		Assert::IsTrue(MainWindow::TaskPanelManager::isDueAfter(dl, 0));
	}

	/// Generic test case for testing if a task is due today (deadline ahead)
	TEST_METHOD(testDueToday2) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl += boost::posix_time::hours(24) + boost::posix_time::minutes(1);
		Assert::IsFalse(MainWindow::TaskPanelManager::isDueAfter(dl, 0));
	}

	/// Generic test case for testing if a task is due today (deadline before)
	TEST_METHOD(testDueToday3) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl -= (boost::posix_time::hours(24) + boost::posix_time::minutes(1));
		Assert::IsFalse(MainWindow::TaskPanelManager::isDueAfter(dl, 0));
	}

	/// Generic test case for testing if a task is due
	/// tomorrow (deadline after)
	TEST_METHOD(testDueTomorrow1) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl += boost::posix_time::hours(24);
		Assert::IsTrue(MainWindow::TaskPanelManager::isDueAfter(dl, 1));
	}

	/// Boundary test case for the equivalence partition for the upper
	/// bound of the lower invalid zone for testing if a task is overdue
	TEST_METHOD(testPastDue1) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl -= boost::posix_time::minutes(1);
		Assert::IsTrue(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Generic test case for testing if a task is overdue
	TEST_METHOD(testPastDue2) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl -= (boost::posix_time::hours(24) + boost::posix_time::minutes(1));
		Assert::IsTrue(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Test if is past due, on the same time
	TEST_METHOD(testPastDue3) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		Assert::IsFalse(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Test if is past due, with deadline 1 minute after
	TEST_METHOD(testPastDue4) {
		MainWindow w;
		Task::Time dl = boost::posix_time::second_clock::local_time();
		dl += boost::posix_time::minutes(1);
		Assert::IsFalse(MainWindow::TaskPanelManager::isPastDue(dl));
	}

	/// Generic task deletion test
	TEST_METHOD(deleteSingleTaskCount) {
		MainWindow w;
		w.clearTasks();
		w.commandTextBox->setPlainText(QString("/add test by Nov 20"));
		w.commandEnterPressed();
		w.commandTextBox->setPlainText(QString("/add test2 by Nov 20"));
		w.commandEnterPressed();
		w.commandTextBox->setPlainText(QString("/delete 1"));
		w.commandEnterPressed();
		Assert::IsTrue(w.ui.taskTreePanel->topLevelItemCount() == 1);
	}

	/// Generic task deletion test
	TEST_METHOD(deleteSingleTaskFind) {
		MainWindow w;
		w.clearTasks();
		w.commandTextBox->setPlainText(QString("/add test by Nov 20"));
		w.commandEnterPressed();
		w.commandTextBox->setPlainText(QString("/add test2 by Nov 20"));
		w.commandEnterPressed();
		w.commandTextBox->setPlainText(QString("/delete 1"));
		w.commandEnterPressed();
		Assert::IsTrue(w.ui.taskTreePanel->findItems(
			QString("1"), Qt::MatchExactly, 1).size() == 0);
	}
};
}  // namespace UnitTests
}  // namespace GUI
}  // namespace You

//@author A0094446X
#include "stdafx.h"
#include "CppUnitTest.h"
//#include <QtTest/QtTest>
#include "You-GUI\main_window.h"
#include "You-GUI\system_tray_manager.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/*
class MainWindowTests : public QObject {
	Q_OBJECT
private slots:
	// Check visibility of elements to show
	void isMainWindowVisible();
	void isCentralWidgetVisible();
	void isCommandEnterButtonVisible();
	void isCommandInputBoxVisible();
	void isStatusBarVisible();
	void isStatusIconVisible();
	void isStatusMessageVisible();
	void isTaskPanelVisible();

	// Check visibility of elements to hide
	void isMainToolBarHidden();
	void isMenuBarHidden();
};

void MainWindowTests::isMainWindowVisible() {
	MainWindow w;
	QVERIFY(w.isVisible());
	w.ui.menuBar->isVisible();
}

void MainWindowTests::isCentralWidgetVisible() {
	MainWindow w;
	QVERIFY(w.ui.centralWidget->isVisible());
}

void MainWindowTests::isTaskPanelVisible() {
	MainWindow w;
	QVERIFY(w.ui.taskTreePanel->isVisible());
}

void MainWindowTests::isCommandEnterButtonVisible() {
	MainWindow w;
	QVERIFY(w.ui.commandEnterButton->isVisible());
}

void MainWindowTests::isCommandInputBoxVisible() {
	MainWindow w;
	QVERIFY(w.ui.commandInputBox->isVisible());
}

void MainWindowTests::isStatusBarVisible() {
	MainWindow w;
	QVERIFY(w.ui.statusBar->isVisible());
}

void MainWindowTests::isStatusIconVisible() {
	MainWindow w;
	QVERIFY(w.ui.statusIcon->isVisible());
}

void MainWindowTests::isStatusMessageVisible() {
	MainWindow w;
	QVERIFY(w.ui.statusMessage->isVisible());
}

void MainWindowTests::isMainToolBarHidden() {
	MainWindow w;
	QVERIFY(!w.ui.mainToolBar->isVisible());
}

void MainWindowTests::isMenuBarHidden() {
	MainWindow w;
	QVERIFY(!w.ui.menuBar->isVisible());
}

#include "You-GUI-Tests\GeneratedFiles\Debug\you_gui_tests.moc"
QTEST_MAIN(MainWindowTests)
*/

QApplication *app;

// Simulate running the main() function
// Sets up the logging facility and the Qt event loop
TEST_MODULE_INITIALIZE(ModuleInitialize) {
	int argc = 1;
	char *argv[] = { "You.exe" };
	app = new QApplication(argc, argv);
}

// Cleans up what we set up// Cleans up what we set up
TEST_MODULE_CLEANUP(ModuleCleanup) {
	app->quit();
	delete app;
}

TEST_CLASS(MainWindowTests) {
public:

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

	TEST_METHOD(isCommandInputBoxVisible) {
		MainWindow w;
		Assert::IsTrue(w.ui.commandInputBox->isVisible());
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
};

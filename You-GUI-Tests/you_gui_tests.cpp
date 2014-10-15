//@author A0094446X
#include "stdafx.h"
#include "CppUnitTest.h"
#include "You-GUI\main_window.h"
#include "You-GUI\system_tray_manager.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

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

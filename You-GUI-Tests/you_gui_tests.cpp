//@author A0094446X
#include "stdafx.h"
#include <QtTest/QtTest>
#include "You-GUI\main_window.h"
#include "You-GUI\system_tray_manager.h"
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

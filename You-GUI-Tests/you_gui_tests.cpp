//@author A0094446X
#include "stdafx.h"
#include <QtTest/QtTest>
#include "You-GUI\main_window.h"
#include "You-GUI\system_tray_manager.h"
class TestMainWindow : public QObject {
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

void TestMainWindow::isMainWindowVisible() {
	MainWindow w;
	QVERIFY(w.isVisible());
	w.ui.menuBar->isVisible();
}

void TestMainWindow::isCentralWidgetVisible() {
	MainWindow w;
	QVERIFY(w.ui.centralWidget->isVisible());
}

void TestMainWindow::isTaskPanelVisible() {
	MainWindow w;
	QVERIFY(w.ui.taskTreePanel->isVisible());
}

void TestMainWindow::isCommandEnterButtonVisible() {
	MainWindow w;
	QVERIFY(w.ui.commandEnterButton->isVisible());
}

void TestMainWindow::isCommandInputBoxVisible() {
	MainWindow w;
	QVERIFY(w.ui.commandInputBox->isVisible());
}

void TestMainWindow::isStatusBarVisible() {
	MainWindow w;
	QVERIFY(w.ui.statusBar->isVisible());
}

void TestMainWindow::isStatusIconVisible() {
	MainWindow w;
	QVERIFY(w.ui.statusIcon->isVisible());
}

void TestMainWindow::isStatusMessageVisible() {
	MainWindow w;
	QVERIFY(w.ui.statusMessage->isVisible());
}

void TestMainWindow::isMainToolBarHidden() {
	MainWindow w;
	QVERIFY(!w.ui.mainToolBar->isVisible());
}

void TestMainWindow::isMenuBarHidden() {
	MainWindow w;
	QVERIFY(!w.ui.menuBar->isVisible());
}

#include "You-GUI-Tests\GeneratedFiles\Debug\you_gui_tests.moc"
QTEST_MAIN(TestMainWindow)

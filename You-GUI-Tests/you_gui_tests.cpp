//@author A0094446X
#include "stdafx.h"
#include <QtTest/QtTest>
#include "You-GUI\main_window.h"
#include "You-GUI\system_tray_manager.h"
class TestMainWindow : public QObject {
	Q_OBJECT
		private slots:
	void testGUI();
};

void TestMainWindow::testGUI() {
	MainWindow w;
	MainWindow::SystemTrayManager stm(&w);
	stm.setup();
	QVERIFY(w.isVisible());
}

#include "You-GUI-Tests\GeneratedFiles\Debug\you_gui_tests.moc"
QTEST_MAIN(TestMainWindow)

//@author A0094446X
#include "stdafx.h"
#include <QtTest/QtTest>
#include "You-GUI\you_main_gui.h"
#include "You-GUI\system_tray_manager.h"
class TestYouMainGUI : public QObject {
	Q_OBJECT
		private slots:
	void testGUI();
};

void TestYouMainGUI::testGUI() {
	YouMainGUI w;
	YouMainGUI::SystemTrayManager stm(&w);
	stm.setup();
	QVERIFY(w.isVisible());
}

#include "You-GUI-Tests\GeneratedFiles\Debug\you_gui_tests.moc"
QTEST_MAIN(TestYouMainGUI)

#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "../You-GUI/main_window.h"
#include "../You-GUI/tray_hotkey_handler.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	You::GUI::MainWindow w;
	w.show();
	You::GUI::Thread::TrayHotkeyHandler t;
	t.start();
	return a.exec();
}

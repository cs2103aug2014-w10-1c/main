//@author A0094446X
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "../You-GUI/main_window.h"

int main(int argc, char *argv[]) {
	QSharedMemory shared("QtApplicationUniqueInstanceYouGUI");
	if (shared.create(512, QSharedMemory::ReadWrite) == false) {
		exit(0);
	}

	QApplication a(argc, argv);
	You::GUI::MainWindow w;
	w.show();
	return a.exec();
}

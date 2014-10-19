#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "../You-GUI/main_window.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	You::GUI::MainWindow w;
	w.show();
	return a.exec();
}

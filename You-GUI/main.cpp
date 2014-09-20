#include "stdafx.h"
#include "you_main_gui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	YouMainGUI w;
	w.show();
	return a.exec();
}

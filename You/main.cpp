#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "../You-GUI/you_main_gui.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	YouMainGUI w;
	w.show();
	return a.exec();
}

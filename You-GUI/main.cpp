#include "stdafx.h"
#include "yougui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	YouGUI w;
	w.show();
	return a.exec();
}

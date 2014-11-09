//@author A0094446X
#include "stdafx.h"
#include <memory>
#include <QApplication>
#include <QList>
#include "session_manager.h"

namespace You {
namespace GUI {

MainWindow::SessionManager::~SessionManager() {
	saveSession();
}

void MainWindow::SessionManager::setup() {
	loadSession();
}

void MainWindow::SessionManager::loadSession() {
	QSettings settings("session.txt", QSettings::IniFormat);
	settings.beginGroup("MainWindow");
	parentGUI->resize(settings.value("size", QSize(572, 528)).toSize());
	parentGUI->move(settings.value("pos", QPoint(200, 200)).toPoint());
	if (settings.value("maximized", false).toBool()) {
		parentGUI->setWindowState(
			parentGUI->windowState() | Qt::WindowMaximized);
	} else {
		parentGUI->setWindowState(
			parentGUI->windowState() & (~Qt::WindowMaximized));
	}
	parentGUI->ui.taskTreePanel->header()->restoreState(settings.value("width").toByteArray());
	//settings.beginReadArray("colWidths");
	//for (int i = 0; i < parentGUI->ui.taskTreePanel->columnCount(); i++) {
	//	settings.setArrayIndex(i);
	//	int width = settings.value("width").toInt();
	//	parentGUI->ui.taskTreePanel->setColumnWidth(i, width);
	//	qDebug() << width;
	//}
	//settings.endArray();

	settings.endGroup();
}

void MainWindow::SessionManager::saveSession() {
	QSettings settings("session.txt", QSettings::IniFormat);
	settings.beginGroup("MainWindow");
	settings.setValue("size", parentGUI->size());
	settings.setValue("pos", parentGUI->pos());
	settings.setValue("maximized", parentGUI->isMaximized());
	settings.setValue("width", parentGUI->ui.taskTreePanel->header()->saveState());
	/*
	settings.beginWriteArray("colWidths");
	for (int i = 0; i < parentGUI->ui.taskTreePanel->columnCount(); i++) {
		settings.setArrayIndex(i);
		int width = parentGUI->ui.taskTreePanel->columnWidth(i);
		settings.setValue("width", width);
		qDebug() << width;
	}
	settings.endArray();*/
	settings.endGroup();
}

}  // namespace GUI
}  // namespace You

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
	int size = settings.beginReadArray("idList");
	for (int i = 0; i < size; i++) {
		settings.setArrayIndex(i);
		taskIDs.push_back(settings.value("ID").toLongLong());
	}
	settings.endArray();
	settings.beginGroup("MainWindow");
	parentGUI->resize(settings.value("size", QSize(400, 400)).toSize());
	parentGUI->move(settings.value("pos", QPoint(200, 200)).toPoint());
	if (settings.value("maximized", false).toBool()) {
		parentGUI->setWindowState(
			parentGUI->windowState() | Qt::WindowMaximized);
	} else {
		parentGUI->setWindowState(
			parentGUI->windowState() & (~Qt::WindowMaximized));
	}
	settings.endGroup();
}

void MainWindow::SessionManager::saveSession() {
	QSettings settings("session.txt", QSettings::IniFormat);
	settings.beginWriteArray("idList");
	for (int i = 0; i < taskIDs.size(); i++) {
		settings.setArrayIndex(i);
		settings.setValue("ID", taskIDs.at(i));
	}
	settings.endArray();
	settings.beginGroup("MainWindow");
	settings.setValue("size", parentGUI->size());
	settings.setValue("pos", parentGUI->pos());
	settings.setValue("maximized", parentGUI->isMaximized());
	settings.endGroup();
}

}  // namespace GUI
}  // namespace You

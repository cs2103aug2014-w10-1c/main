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
	settings.beginGroup("MainWindow");
	settings.setValue("size", parentGUI->size());
	settings.setValue("pos", parentGUI->pos());
	settings.setValue("maximized", parentGUI->isMaximized());
	settings.endGroup();
}

}  // namespace GUI
}  // namespace You

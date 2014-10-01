//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "session_manager.h"

YouMainGUI::SessionManager::~SessionManager() {
	saveSession();
}

void YouMainGUI::SessionManager::setup() {
	loadSession();
}

void YouMainGUI::SessionManager::loadSession() {
	QSettings settings("You", "You");
	settings.beginGroup("MainWindow");
	parentGUI->resize(settings.value("size", QSize(400, 400)).toSize());
	parentGUI->move(settings.value("pos", QPoint(200, 200)).toPoint());
	settings.endGroup();
}

void YouMainGUI::SessionManager::saveSession() {
	QSettings settings("You", "You");
	settings.beginGroup("MainWindow");
	settings.setValue("size", parentGUI->size());
	settings.setValue("pos", parentGUI->pos());
	settings.endGroup();
}

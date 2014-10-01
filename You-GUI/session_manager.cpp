//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "session_manager.h"
#include <memory>
YouMainGUI::SessionManager::~SessionManager() {
	saveSession();
}

void YouMainGUI::SessionManager::setup() {
	loadSession();
}

void YouMainGUI::SessionManager::loadSession() {
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
	settings.endGroup();
}

void YouMainGUI::SessionManager::saveSession() {
	// Capture list of IDs
	QList<int64_t> idList;
	for (int i = 0; i < taskIDs.size(); i++) {
		idList.append(taskIDs.at(i));
	}
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
	settings.endGroup();
}

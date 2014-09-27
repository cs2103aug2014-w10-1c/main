#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "you_main_gui.h"

YouMainGUI::SystemTrayManager::~SystemTrayManager() {
}

void YouMainGUI::SystemTrayManager::setup() {}

void YouMainGUI::SystemTrayManager::setIcon() {
	QIcon icon("icon.png");
	trayIcon.setIcon(icon);
	parentGUI->setWindowIcon(icon);
	trayIcon.show();
}

void YouMainGUI::SystemTrayManager::iconActivated(
	QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		if (!parentGUI->YouMainGUI::isVisible()) {
			parentGUI->YouMainGUI::setVisible(false);
		} else {
			parentGUI->YouMainGUI::setVisible(true);
		}
	case QSystemTrayIcon::DoubleClick:
		if (!parentGUI->YouMainGUI::isVisible()) {
			parentGUI->YouMainGUI::setVisible(false);
		} else {
			parentGUI->YouMainGUI::setVisible(true);
		}
		break;
	default: {}
	}
}

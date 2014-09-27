#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "you_main_gui.h"
#include "system_tray_manager.h"

YouMainGUI::SystemTrayManager::~SystemTrayManager() {
}

void YouMainGUI::SystemTrayManager::setup() {
	createActions();
	setIcon();
}

void YouMainGUI::SystemTrayManager::setIcon() {
	QIcon icon("icon.png");
	trayIcon.setIcon(icon);
	parentGUI->setWindowIcon(icon);
	trayIcon.show();
	trayIconMenu = new QMenu(parentGUI);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);
	trayIcon.setContextMenu(trayIconMenu);
	parentGUI->setVisible(true);
	connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void YouMainGUI::SystemTrayManager::iconActivated(
	QSystemTrayIcon::ActivationReason reason) {
	switch (reason) {
	case QSystemTrayIcon::Trigger:
		parentGUI->setVisible(false);
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
void YouMainGUI::SystemTrayManager::createActions() {
	minimizeAction = new QAction(tr("Minimize"), parentGUI);
	connect(minimizeAction, SIGNAL(triggered()), parentGUI, SLOT(hide()));

	maximizeAction = new QAction(tr("Maximize"), parentGUI);
	connect(maximizeAction, SIGNAL(triggered()), parentGUI, SLOT(showMaximized()));

	restoreAction = new QAction(tr("Restore"), parentGUI);
	connect(restoreAction, SIGNAL(triggered()), parentGUI, SLOT(showNormal()));

	quitAction = new QAction(tr("Quit"), parentGUI);
	connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

//@author A0094446X
#include "stdafx.h"
#include <QApplication>
#include <QList>
#include "main_window.h"
#include "system_tray_manager.h"
namespace You {
namespace GUI {

MainWindow::SystemTrayManager::~SystemTrayManager() {
}

void MainWindow::SystemTrayManager::setup() {
	createActions();
	setIcon();
	makeContextMenu();
	connectTrayActivatedSlot();
	parentGUI->setVisible(true);
}

void MainWindow::SystemTrayManager::setIcon() {
	QIcon icon(":/Icon.png");
	trayIcon.setIcon(icon);
	parentGUI->setWindowIcon(icon);
	trayIcon.show();
}

void MainWindow::SystemTrayManager::makeContextMenu() {
	trayIconMenu = new QMenu(parentGUI);
	trayIconMenu->addAction(minimizeAction);
	trayIconMenu->addAction(maximizeAction);
	trayIconMenu->addAction(restoreAction);
	trayIconMenu->addSeparator();
	trayIconMenu->addAction(quitAction);
	trayIcon.setContextMenu(trayIconMenu);
}

void MainWindow::SystemTrayManager::connectTrayActivatedSlot() {
	connect(&trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
		this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::SystemTrayManager::iconActivated(
	QSystemTrayIcon::ActivationReason reason) {
	if (reason == QSystemTrayIcon::Trigger) {
		bool visible = parentGUI->isVisible();
		bool minimized = parentGUI->isMinimized();
		assert((visible && minimized) || (visible && !minimized) ||
			(!visible));
		Qt::WindowStates toggleState
			(parentGUI->windowState() & ~Qt::WindowMinimized);
		if (visible && minimized) {
			parentGUI->setWindowState(toggleState | Qt::WindowActive);
			parentGUI->ui.commandInputBox->setFocus(
				Qt::FocusReason::ActiveWindowFocusReason);
		} else if (visible && !minimized) {
			parentGUI->setWindowState(toggleState | Qt::WindowActive);
			parentGUI->hide();
		} else if (!visible) {
			parentGUI->show();
			parentGUI->setWindowState(toggleState | Qt::WindowActive);
			parentGUI->ui.commandInputBox->setFocus(
				Qt::FocusReason::ActiveWindowFocusReason);
		}
	}
}

void MainWindow::SystemTrayManager::createActions() {
	minimizeAction = new QAction(tr("Minimize"), parentGUI);
	connect(minimizeAction, SIGNAL(triggered()), parentGUI, SLOT(hide()));

	maximizeAction = new QAction(tr("Maximize"), parentGUI);
	connect(maximizeAction, SIGNAL(triggered()), parentGUI, SLOT(showMaximized()));

	restoreAction = new QAction(tr("Restore"), parentGUI);
	connect(restoreAction, SIGNAL(triggered()), parentGUI, SLOT(showNormal()));

	quitAction = new QAction(tr("Quit"), parentGUI);
	connect(quitAction, SIGNAL(triggered()),
		parentGUI, SLOT(applicationExitRequested()));
}

}  // namespace GUI
}  // namespace You

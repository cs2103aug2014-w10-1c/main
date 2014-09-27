#pragma once
#ifndef YOU_GUI_SYSTEM_TRAY_MANAGER_H_
#define YOU_GUI_SYSTEM_TRAY_MANAGER_H_
#include <QApplication>
#include <QList>
#include "you_main_gui.h"

class YouMainGUI::SystemTrayManager : public YouMainGUI::BaseManager{
	Q_OBJECT
	friend class YouMainGUI;
public:
	explicit SystemTrayManager(YouMainGUI * const parentGUI)
		: BaseManager(parentGUI) {}
	~SystemTrayManager();
	QAction *minimizeAction;
	QAction *maximizeAction;
	QAction *restoreAction;
	QAction *quitAction;
	QMenu* trayIconMenu;

private:
	void setup();
	/// System Tray functions
	/// Defines and sets the tray icon. Called in the constructor.
	void setIcon();
	void createActions();
	/// System tray icon object that adds an icon to the tray.
	QSystemTrayIcon trayIcon;

	/// QT's signal/slot mechanism for tray icon activation.
	void iconActivated(QSystemTrayIcon::ActivationReason reason);

	/// Icon image for system tray.
	QIcon icon;
};

#endif  // YOU_GUI_SYSTEM_TRAY_MANAGER_H_

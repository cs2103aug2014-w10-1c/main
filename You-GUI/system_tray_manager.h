//@author A0094446X
#pragma once
#ifndef YOU_GUI_SYSTEM_TRAY_MANAGER_H_
#define YOU_GUI_SYSTEM_TRAY_MANAGER_H_
#include <QApplication>
#include "base_manager.h"

/// The component that handles all interactions with the tray icon. It handles
/// the window state of the application based on commands sent to the tray
/// manager. It inherits from the BaseManager class.
class YouMainGUI::SystemTrayManager : public YouMainGUI::BaseManager{
	Q_OBJECT
	friend class YouMainGUI;
public:
	explicit SystemTrayManager(YouMainGUI * const parentGUI)
		: BaseManager(parentGUI) {}
	~SystemTrayManager();
	/// Defines and sets functionality of the tray icon. Called in the
	/// constructor.
	void setup();

private:
	/// Minimize action in the context menu in system tray
	QAction *minimizeAction;
	/// Maximize action in the context menu in system tray
	QAction *maximizeAction;
	/// Restore action in the context menu in system tray
	QAction *restoreAction;
	/// Quit action in the context menu in system tray
	QAction *quitAction;
	/// Tray icon context menu
	QMenu* trayIconMenu;

	/// Declares and defines the image of the tray icon.
	void setIcon();

	/// Creates actions for the context menu.
	void createActions();

	/// Adds created actions to the context menu.
	void makeContextMenu();

	/// Connects the signal/slot of the tray icon
	void connectTrayActivatedSlot();

	/// System tray icon object that adds an icon to the tray.
	QSystemTrayIcon trayIcon;

	/// Icon image for system tray.
	QIcon icon;

	private slots:
	/// QT's signal/slot mechanism for tray icon activation. Implements
	/// show/hide behavior on click.
	void iconActivated(QSystemTrayIcon::ActivationReason reason);
};

#endif  // YOU_GUI_SYSTEM_TRAY_MANAGER_H_

#pragma once
#ifndef YOU_GUI_SESSION_MANAGER_H_
#define YOU_GUI_SESSION_MANAGER_H_
#include <QApplication>
#include "base_manager.h"

/// The component that handles all session-related data of the application. It
/// handles basics such as the window state of the application and task sets
/// previously displayed, saving the state on program exit, and restoring them
/// when the program is started again. It inherits from the BaseManager class.
class YouMainGUI::SessionManager : public YouMainGUI::BaseManager{
	Q_OBJECT
	friend class YouMainGUI;
public:
	explicit SessionManager(YouMainGUI * const parentGUI)
		: BaseManager(parentGUI) {}
	~SessionManager();
private:
	void setup();
	/// Loads the previous state of the GUI. Called during constructor.
	void loadSession();

	/// Saves the state of the GUI before closing. Called during closeEvent.
	void saveSession();
};

#endif  // YOU_GUI_SESSION_MANAGER_H_

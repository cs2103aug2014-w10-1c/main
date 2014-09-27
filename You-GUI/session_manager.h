#pragma once
#ifndef YOU_GUI_SESSION_MANAGER_H_
#define YOU_GUI_SESSION_MANAGER_H_
#include <QApplication>
#include <QList>
#include "you_main_gui.h"

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

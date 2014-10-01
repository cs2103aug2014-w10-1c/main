//@author A0094446X
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
	friend class NLPManager;
public:
	/// Constructor inherited from BaseManager.
	explicit SessionManager(YouMainGUI * const parentGUI)
		: BaseManager(parentGUI) {}
	/// Destructor.
	~SessionManager();

	/// Wrapper function to load session
	void setup();

	/// The list of taskIDs that will be filled on program start from the
	/// session file, to be passed to the NLP manager. Mechanism to pass is
	/// still being considered. This will also contain the list of IDs to be
	/// written to the session file.
	std::vector<int64_t> taskIDs;

private:
	/// Loads the previous state of the GUI. Called during constructor.
	void loadSession();

	/// Saves the state of the GUI before closing. Called during closeEvent.
	void saveSession();
};

#endif  // YOU_GUI_SESSION_MANAGER_H_

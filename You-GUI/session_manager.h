//@author A0094446X
#pragma once
#ifndef YOU_GUI_SESSION_MANAGER_H_
#define YOU_GUI_SESSION_MANAGER_H_
#include <QApplication>
#include "base_manager.h"

namespace You {
namespace GUI {

namespace UnitTests { class MainWindowTests; }

using Task = You::Controller::Task;

/// The component that handles all session-related data of the application. It
/// handles basics such as the window state of the application,
/// saving the state on program exit, and restoring them
/// when the program is started again. It inherits from the BaseManager class.
class MainWindow::SessionManager : public MainWindow::BaseManager{
	Q_OBJECT
	friend class UnitTests::MainWindowTests;
public:
	/// Constructor inherited from BaseManager.
	explicit SessionManager(MainWindow * const parentGUI)
		: BaseManager(parentGUI) {}

	/// Destructor.
	~SessionManager();

	/// Wrapper function to load session
	void setup();

private:
	/// Loads the previous state of the GUI. Called during constructor.
	void loadSession();

	/// Saves the state of the GUI before closing. Called during closeEvent.
	void saveSession();
};

}  // namespace GUI
}  // namespace You

#endif  // YOU_GUI_SESSION_MANAGER_H_

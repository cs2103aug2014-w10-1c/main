//@author A0094446X
#pragma once
#ifndef YOU_GUI_BASE_MANAGER_H_
#define YOU_GUI_BASE_MANAGER_H_
#include "main_window.h"
namespace You {
namespace GUI {

/// The class from which all components inherit.
class MainWindow::BaseManager : public QObject{
	Q_OBJECT
public:
	/// The constructor that assigns to the manager the parent GUI.
	explicit BaseManager(MainWindow * parentGUI);

	/// Tasks needed to be run on startup
	void setup();

protected:
	/// Pointer to the main GUI
	MainWindow * const parentGUI;

private:
	/// BaseManager constructor. Not used.
	BaseManager() = delete;
};

}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_BASE_MANAGER_H_

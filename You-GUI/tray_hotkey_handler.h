//@author A0094446X
#pragma once
#ifndef YOU_GUI_TRAY_HOTKEY_HANDLER_H_
#define YOU_GUI_TRAY_HOTKEY_HANDLER_H_
#include <QThread>
#include "windows.h"

namespace You {
namespace GUI {
namespace Thread {

/// TrayHotkeyHandler inherits from QThread, and runs on a separate thread
/// as a listener for a hotkey event. It registers the hotkey before looping.
class TrayHotkeyHandler : public QThread {
	Q_OBJECT
public:
	/// Constructor.
	TrayHotkeyHandler();

signals:
	/// Signals to SystemTrayManager that hotkey has been pressed
	void hotkeyClicked(QSystemTrayIcon::ActivationReason);

protected:
	/// Loops in its own thread to check if hotkey is pressed.
	void run();

private:
	/// Hotkey to be pressed in conjunction with Win-Key. 0x59 is 'Y'
	const int hotkey = 0x59;
};

}  // namespace Thread
}  // namespace GUI
}  // namespace You

#endif  // YOU_GUI_TRAY_HOTKEY_HANDLER_H_

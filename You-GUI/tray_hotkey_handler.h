//@author A0094446X
#pragma once
#ifndef YOU_GUI_TRAY_HOTKEY_HANDLER_H_
#define YOU_GUI_TRAY_HOTKEY_HANDLER_H_
#include <QThread>
#include "windows.h"
namespace You {
namespace GUI {
namespace Thread {

class TrayHotkeyHandler : public QThread {
	Q_OBJECT
public:
	TrayHotkeyHandler();
signals:
	void hotkeyClicked(QSystemTrayIcon::ActivationReason);
protected:
	void run();
};

}  // namespace Thread
}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_TRAY_HOTKEY_HANDLER_H_

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
	TrayHotkeyHandler() {
		moveToThread(this);
	}
protected:
	void run() {
		while (1) {
			Sleep(10);
			if (GetAsyncKeyState(0x59) == -32767)
				if (GetAsyncKeyState(0x5B) == -32767)
					qDebug() << "lol";
		}
	}
};


}  // namespace Thread
}  // namespace GUI
}  // namespace You
#endif  // YOU_GUI_TRAY_HOTKEY_HANDLER_H_

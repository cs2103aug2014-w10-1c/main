#pragma once
#ifndef YOU_GUI_TRAY_HOTKEY_HANDLER_H_
#define YOU_GUI_TRAY_HOTKEY_HANDLER_H_
#include <QThread>
#include "windows.h"
namespace You {
namespace GUI {
class TrayHotkeyHandler : public QThread {
	Q_OBJECT
public:
	TrayHotkeyHandler() {
		moveToThread(this);
	};
	void run();
};


void TrayHotkeyHandler::run()
{
	while (1)
	{
		Sleep(40);
	}
}

}
}

#endif  // YOU_GUI_TRAY_HOTKEY_HANDLER_H_

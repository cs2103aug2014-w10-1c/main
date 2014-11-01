//@author A0094446X
#include "stdafx.h"
#include <windows.h>
#include "tray_hotkey_handler.h"

namespace You {
namespace GUI {
namespace Thread {

TrayHotkeyHandler::TrayHotkeyHandler() {
	moveToThread(this);
}

void TrayHotkeyHandler::run() {
	RegisterHotKey(0, 0, MOD_WIN, hotkey);
	MSG msg;
	// Allows Qt to send ActivationReason as a signal type
	qRegisterMetaType<QSystemTrayIcon::ActivationReason>(
		"QSystemTrayIcon::ActivationReason");

	while (1) {
		msg = { 0 };
		GetMessage(&msg, 0, 0, 0);
		if (msg.message == WM_HOTKEY) {
			emit hotkeyClicked(QSystemTrayIcon::Trigger);
		}
		DispatchMessage(&msg);
	}
}

}  // namespace Thread
}  // namespace GUI
}  // namespace You

//@author A0094446X
#include "stdafx.h"
#include "tray_hotkey_handler.h"

namespace You {
namespace GUI {
namespace Thread {

TrayHotkeyHandler::TrayHotkeyHandler() {
	moveToThread(this);
}

void TrayHotkeyHandler::run() {
	RegisterHotKey(0, 0, MOD_WIN, 0x59);
	qRegisterMetaType<QSystemTrayIcon::ActivationReason>(
		"QSystemTrayIcon::ActivationReason");
	MSG msg;
	while (1) {
		Sleep(40);
		msg = { 0 };
		PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
		if (msg.message == WM_HOTKEY) {
			emit hotkeyClicked(QSystemTrayIcon::Trigger);
		}
		DispatchMessage(&msg);
	}
}

}  // namespace Thread
}  // namespace GUI
}  // namespace You

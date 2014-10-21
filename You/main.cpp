//@author A0094446X
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "../You-GUI/main_window.h"
#include "../You-GUI/window_title.h"
#include "windows.h"

int main(int argc, char *argv[]) {
	HANDLE hMutex;
	hMutex = CreateMutex(NULL, FALSE, TEXT("YouGUIUniqueApplicationInstance"));
	DWORD m_DwLastError = GetLastError();
	if (m_DwLastError == ERROR_ALREADY_EXISTS) {
		HWND hwnd = FindWindow(NULL, WINDOW_TITLE.c_str());
		SetForegroundWindow(hwnd);
		return 0;
	} else {
		QApplication a(argc, argv);
		You::GUI::MainWindow w;
		w.show();
		return a.exec();
	}
}

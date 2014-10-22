//@author A0094446X
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "../You-GUI/main_window.h"
#include "../You-GUI/window_title.h"

#ifdef _WIN32
#include "windows.h"
int loadSingleApplication(int argc, char *argv[]);
#else
int loadApplication(int argc, char *argv[]);
#endif

int main(int argc, char *argv[]) {
#ifdef _WIN32
	return loadSingleApplication(argc, argv);
#else
	return loadApplication(argc, argv);
#endif
}

#ifdef _WIN32
int loadSingleApplication(int argc, char *argv[]) {
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
#else
int loadApplication(int argc, char *argv[]) {
	QApplication a(argc, argv);
	You::GUI::MainWindow w;
	w.show();
	return a.exec();
}
#endif

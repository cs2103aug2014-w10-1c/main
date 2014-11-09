//@author A0094446X
#include "stdafx.h"
#include <QtWidgets/QApplication>
#include "You-Utils/log.h"
#include "log_sink.h"
#include "exceptions.h"
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

int loadApplication(int argc, char *argv[]) {
	QApplication a(argc, argv);
	try {
		std::shared_ptr<You::GUILogSink> ls =
			std::make_shared<You::GUILogSink>();
		You::Utils::Log::setSink(ls);
	}
	catch (You::Exception::InitLogFileFailedException &e) {
		qDebug() << "Failed to initialize log file.";
	}
	You::GUI::MainWindow w;
	w.show();
	return a.exec();
}

int loadSingleApplication(int argc, char *argv[]) {
	HANDLE hMutex;
	hMutex = CreateMutex(NULL, FALSE, TEXT("YouGUIUniqueApplicationInstance"));
	DWORD m_DwLastError = GetLastError();
	if (m_DwLastError == ERROR_ALREADY_EXISTS) {
		HWND hwnd = FindWindow(NULL, WINDOW_TITLE.c_str());
		SetForegroundWindow(hwnd);
		return 0;
	} else {
		return loadApplication(argc, argv);
	}
}

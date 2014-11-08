//@author A0094446X
#include "stdafx.h"
#include <QTextStream>
#include <QFile>
#include "gui_log_sink.h"

namespace You {
namespace GUI {

const QString LOG_FILENAME = "you.log";

GUILogSink::GUILogSink() {}
GUILogSink::~GUILogSink() {}

void GUILogSink::onLog(LogSeverity severity,
	const std::wstring& category,
	const std::wstring& message) {
	QString filename = LOG_FILENAME;
	QFile file(filename);

	std::wstringstream severityStr;
	switch (severity) {
	case LogSeverity::DEBUG:
		severityStr << L"<<Debug>> ";
		break;
	case LogSeverity::INFO:
		severityStr << L"<<Information>> ";
		break;
	case LogSeverity::WARNING:
		severityStr << L"<<Warning>> ";
		break;
	case LogSeverity::ERROR:
		severityStr << L"<<Error>> ";
		break;
	case LogSeverity::CRITICAL:
		severityStr << L"<<Critical>> ";
		break;
	default:
		severityStr << L"<<Log Message Type not recognized.>>";
		assert(false);
		break;
	}
	severityStr << category << message;
	if (file.open(QIODevice::ReadWrite | QIODevice::Append)) {
		QTextStream stream(&file);
		stream << QString::fromStdWString(severityStr.str());
	}
}

}  // namespace GUI
}  // namespace You

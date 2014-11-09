//@author A0094446X
#include "stdafx.h"
#include <QTextStream>
#include <QFile>
#include "log_sink.h"
#include "exceptions.h"

namespace You {

const QString GUILogSink::LOG_FILENAME = "you.log";

GUILogSink::GUILogSink() : file(new QFile(LOG_FILENAME)) {
	if (!file->open(QIODevice::ReadWrite | QIODevice::Append)) {
		throw Exception::InitLogFileFailedException();
	}
}

void GUILogSink::onLog(LogSeverity severity,
	const std::wstring& category,
	const std::wstring& message) {

	QTextStream severityStr(file.get());
	switch (severity) {
	case LogSeverity::DEBUG:
		severityStr << "<<Debug>> ";
		break;
	case LogSeverity::INFO:
		severityStr << "<<Information>> ";
		break;
	case LogSeverity::WARNING:
		severityStr << "<<Warning>> ";
		break;
	case LogSeverity::ERROR:
		severityStr << "<<Error>> ";
		break;
	case LogSeverity::CRITICAL:
		severityStr << "<<Critical>> ";
		break;
	default:
		severityStr << "<<Log Message Type not recognized.>>";
		assert(false);
		break;
	}
	severityStr << QString::fromStdWString(category)
		<< QString::fromStdWString(message) << QString("\n");
}

}  // namespace You

//@author A0094446X
#pragma once
#ifndef YOU_GUI_GUI_LOG_SINK_H_
#define YOU_GUI_GUI_LOG_SINK_H_
#include "You-Utils/log.h"
#include "You-Utils/log/log_severity.h"

namespace You {
namespace GUI {

using LogSeverity = You::Utils::LogSeverity;

/// The log sink, inheriting from You::Utils::LogSink. It collects log messages
/// from the rest of the components and writes them into a log file.
class GUILogSink : public You::Utils::LogSink {
public:
	~GUILogSink();
	GUILogSink();
	void onLog(LogSeverity severity,
		const std::wstring& category,
		const std::wstring& message);

private:
	const QString LOG_FILENAME;
};

}  // namespace GUI
}  // namespace You

#endif  // YOU_GUI_GUI_LOG_SINK_H_

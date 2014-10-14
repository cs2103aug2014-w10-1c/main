//@author A0097630B
#include "stdafx.h"
#include "log.h"

namespace You {
namespace Utils {

Logger Log::debug(LogSeverity::DEBUG);
LogSeverity Log::logLevel;
std::shared_ptr<LogSink> Log::sink;

void Log::write(
	LogSeverity severity,
	const std::wstring& category,
	const std::wstring& message) {
	if (severity >= Log::logLevel && sink) {
		sink->onLog(severity, category, message);
	}
}

}  // namespace Utils
}  // namespace You

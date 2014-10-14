//@author A0097630B
#include "stdafx.h"
#include "log.h"

namespace You {
namespace Utils {

Logger Log::debug(LogSeverity::DEBUG);

Log& Log::get() {
	static Log log;
	return log;
}

void Log::write(
	LogSeverity severity,
	const std::wstring& category,
	const std::wstring& message) {
	if (sink) {
		sink->onLog(severity, category, message);
	}
}

}  // namespace Utils
}  // namespace You

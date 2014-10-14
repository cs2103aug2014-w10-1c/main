//@author A0097630B
#include "stdafx.h"
#include "log.h"
#include "logger.h"

namespace You {
namespace Utils {

Logger::Logger(LogSeverity severity)
: severity(severity) {
}

bool Logger::isActive() const {
	return Log::getLogLevel() <= severity;
}

void Logger::write(const std::wstring& message, const std::wstring& category) {
	Log::write(severity, category, message);
}

}  // namespace Utils
}  // namespace You

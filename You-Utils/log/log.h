//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOG_H_
#define YOU_UTILS_LOG_LOG_H_

#include <memory>

#include "logger.h"
#include "log_severity.h"
#include "log_sink.h"

namespace You {
namespace Utils {

class Log {
	friend class Logger;

public:
	static Log& get();

	static Logger debug;
	static Logger info;
	static Logger warning;
	static Logger error;
	static Logger critical;
	
	inline LogSeverity getLogLevel() const {
		return logLevel;
	}

	void setLogLevel(LogSeverity severity) {
		Log::logLevel = severity;
	}

	void setSink(std::shared_ptr<LogSink> sink) {
		Log::sink = sink;
	}

private:
	Log() = default;

	void write(
		LogSeverity severity,
		const std::wstring& category,
		const std::wstring& message);

private:
	LogSeverity logLevel;
	std::shared_ptr<LogSink> sink;
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_H_

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

/// The main Logging class.
///
/// This handles all log requests from the various components, and manages the
/// active log sink as well as the active log level.
///
/// This is NOT a singleton because then the syntax for writing to the log would
/// be very clumsy, i.e.
///
/// Log::get().debug << "message"
class Log {
	friend class Logger;

public:
	/// The logger which prints debug-level messages.
	///
	/// \see LogSeverity
	static Logger debug;

	/// The logger which prints info-level messages.
	///
	/// \see LogSeverity
	static Logger info;

	/// The logger which prints warning-level messages.
	///
	/// \see LogSeverity
	static Logger warning;

	/// The logger which prints error-level messages.
	///
	/// \see LogSeverity
	static Logger error;

	/// The logger which prints critical-level messages.
	///
	/// \see LogSeverity
	static Logger critical;
	
	/// Gets the active log level.
	///
	/// The active log level is the minimum severity for which messages must be
	/// before it will be written to the log sink.
	///
	/// \return The active log level.
	inline static LogSeverity getLogLevel() {
		return logLevel;
	}

	/// Sets the active log level.
	///
	/// \see getLogLevel
	/// See this to understand about log levels.
	///
	/// \param[in] severity The new active log level to set.
	static void setLogLevel(LogSeverity severity) {
		Log::logLevel = severity;
	}

	/// Sets the active log sink.
	///
	/// The active log sink receives all the messages which are at or above the
	/// active log level.
	///
	/// \param[in] sink The new sink to use.
	static void setSink(std::shared_ptr<LogSink> sink) {
		Log::sink = sink;
	}

private:
	Log() = delete;

	/// Writes the given message to the log sink.
	///
	/// \param[in] severity The severity of the message.
	/// \param[in] category The category of the message.
	/// \param[in] message The message for the log entry.
	static void write(
		LogSeverity severity,
		const std::wstring& category,
		const std::wstring& message);

private:
	/// The active log level.
	static LogSeverity logLevel;
	
	/// The active log sink.
	static std::shared_ptr<LogSink> sink;
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_H_

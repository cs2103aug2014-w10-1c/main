//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOG_SINK_H_
#define YOU_UTILS_LOG_LOG_SINK_H_

#include "log_severity.h"

namespace You {
namespace Utils {

/// This represents the functions an abstract Log Sink must implement. This is
/// an implementation of the Observer pattern.
class LogSink {
public:
	virtual ~LogSink() = default;

	/// The method which will be called when a new log message is received.
	///
	/// \param[in] severity The severity of the message.
	/// \param[in] category The category of the message. This can be an empty
	///                     string for the default category.
	/// \param[in] message The message which was logged.
	virtual void onLog(LogSeverity severity,
		const std::wstring& category,
		const std::wstring& message) = 0;
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_SINK_H_

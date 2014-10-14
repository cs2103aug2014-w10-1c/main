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
	virtual void onLog(LogSeverity severity,
		const std::wstring& category,
		const std::wstring& message) = 0;
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_SINK_H_

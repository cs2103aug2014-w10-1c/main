//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOG_SEVERITY_H_
#define YOU_UTILS_LOG_LOG_SEVERITY_H_

namespace You {
namespace Utils {

/// The severity of log messages.
enum class LogSeverity {
	/// The message is of DEBUG severity.
	DEBUG = 0,

	/// The message is of INFO severity.
	INFO,

	/// The message is of WARNING severity.
	WARNING,

	/// The message is of ERROR severity.
	ERROR,

	/// The message is of CRITICAL severity.
	CRITICAL
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_SEVERITY_H_

//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOG_SEVERITY_H_
#define YOU_UTILS_LOG_LOG_SEVERITY_H_

namespace You {
namespace Utils {
		
enum class LogSeverity {
	DEBUG = 0,
	INFO,
	WARNING,
	ERROR,
	CRITICAL
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_SEVERITY_H_

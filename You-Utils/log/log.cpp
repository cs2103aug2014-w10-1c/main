//@author A0097630B
#include "stdafx.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
#include "log.h"

namespace You {
namespace Utils {

Logger Log::debug(LogSeverity::DEBUG);
Logger Log::info(LogSeverity::INFO);
Logger Log::warning(LogSeverity::WARNING);
Logger Log::error(LogSeverity::ERROR);
Logger Log::critical(LogSeverity::CRITICAL);

#ifdef _DEBUG
LogSeverity Log::logLevel = LogSeverity::DEBUG;
#else
LogSeverity Log::logLevel = LogSeverity::WARNING;
#endif
std::shared_ptr<LogSink> Log::sink;

namespace {

/// The format for debug messages.
const boost::wformat DEBUG_FORMAT(L"[%1%] <%2%>%3% %4%\n");

/// The format for categories.
const boost::wformat CATEGORY_FORMAT(L"[%1%]");

class DebugLogger : public You::Utils::LogSink {
	void onLog(You::Utils::LogSeverity severity,
		const std::wstring& category,
		const std::wstring& message) override {
		std::wstring categoryString;
		if (!category.empty()) {
			categoryString = (boost::wformat(CATEGORY_FORMAT) % category).str();
		}

		OutputDebugStringW(
			(boost::wformat(DEBUG_FORMAT) %
			boost::posix_time::second_clock::local_time() %
			static_cast<int>(severity) %
			category %
			message).str().c_str());
	}
};

class SetDefaultLogger {
public:
	SetDefaultLogger() {
		You::Utils::Log::restoreSink();
	}
};

SetDefaultLogger setDefault;

}  // namespace

void Log::restoreSink() {
	setSink(std::make_shared<DebugLogger>());
}

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

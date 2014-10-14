//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOG_MESSAGE_H_
#define YOU_UTILS_LOG_LOG_MESSAGE_H_

#include <string>
#include <vector>
#include <functional>

namespace You {
namespace Utils {

/// This Logger class deals with logging one message. It is the actual class
/// that constructs log messages.
class LogMessage {
	friend class Logger;

public:
	LogMessage(LogMessage&& rhs);
	~LogMessage();

	template<typename TLazy>
	LogMessage& operator<<(const TLazy& thing) {
		components.emplace_back([&thing]() {
			return boost::lexical_cast<std::wstring>(thing());
		});
	}

	LogMessage& operator<<(const wchar_t* const string) {
		components.emplace_back([&string] { return std::wstring(string); });
		return *this;
	}

	LogMessage& operator<<(std::function<std::wstring()> pickle) {
		components.emplace_back(std::move(pickle));
		return *this;
	}

private:
	LogMessage(Logger& log, std::wstring category = std::wstring());
	LogMessage(const LogMessage&) = delete;
	LogMessage& operator=(const LogMessage&) = delete;

	std::wstring evaluate() const;

private:
	class Logger& logger;
	std::wstring category;
	std::vector<std::function<std::wstring()>> components;
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_MESSAGE_H_

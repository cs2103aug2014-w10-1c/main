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
	/// Move constructor.
	LogMessage(LogMessage&& rhs);

	/// Destructor. Commits the message to the owning logger.
	~LogMessage();

	/// Writes the given thing to the message.
	///
	/// \param[in] thing The message to append. This must be convertible to a
	///                  string, via lexical_cast.
	template<typename TLazy>
	LogMessage& operator<<(const TLazy& thing) {
		components.emplace_back([&thing]() {
			return boost::lexical_cast<std::wstring>(thing());
		});
		return *this;
	}

	/// Writes the given message.
	///
	/// \param[in] thing The message to append. The message is not copied and
	///                  must have a lifetime longer than this.
	LogMessage& operator<<(const wchar_t* const string) {
		components.emplace_back([string] { return std::wstring(string); });
		return *this;
	}

	/// Writes the given message.
	///
	/// \param[in] pickle The function to apply. This must be a nullary function
	///                   returning a string.
	LogMessage& operator<<(std::function<std::wstring()> pickle) {
		components.emplace_back(std::move(pickle));
		return *this;
	}

private:
	/// Constructs a new log message buffer, with the given category.
	///
	/// \param[in] log The logger this message will write to.
	/// \param[in] category The category of the message we are creating.
	LogMessage(Logger& log, std::wstring category = std::wstring());
	LogMessage(const LogMessage&) = delete;
	LogMessage& operator=(const LogMessage&) = delete;

	/// Forces evaluation of the arguments.
	///
	/// \return The complete message.
	std::wstring evaluate() const;

private:
	/// The logger to write to.
	class Logger& logger;

	/// The category of the message.
	std::wstring category;

	/// The unevaluated components of the message.
	std::vector<std::function<std::wstring()>> components;
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_LOG_LOG_MESSAGE_H_

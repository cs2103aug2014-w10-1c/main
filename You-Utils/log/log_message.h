//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOG_MESSAGE_H_
#define YOU_UTILS_LOG_LOG_MESSAGE_H_

#include <string>
#include <vector>
#include <functional>
#include <type_traits>

#include <boost/type_traits/is_fundamental.hpp>

namespace You {
namespace Utils {

/// This Logger class deals with logging one message. It is the actual class
/// that constructs log messages.
class LogMessage {
	friend class Logger;

	/// Template metaprogram to check if T is a functor.
	///
	/// Slightly modified from http://stackoverflow.com/questions/5100015
	template<typename T>
	struct is_callable {
		template<typename C>  // detect regular operator()
		static char test(decltype(&C::operator()));

		template<typename C>  // worst match
		static char(&test(C))[2];  // NOLINT(readability/casting)

		static const bool value = !boost::is_fundamental<T>::value &&
			!boost::is_array<T>::value &&
			sizeof(test<T>(0)) == 1;
	};

public:
	/// Move constructor.
	LogMessage(LogMessage&& rhs);

	/// Destructor. Commits the message to the owning logger.
	~LogMessage();

	/// Writes the given message.
	///
	/// \param[in] pickle The function to apply. This must be a nullary function
	///                   returning a value convertible to std::wstring using
	///                   lexical_cast.
	template<
		typename TLazy,
		typename = std::enable_if_t<is_callable<TLazy>::value>>
	LogMessage& operator<<(TLazy pickle) {
		components.emplace_back([pickle]() {
			return boost::lexical_cast<std::wstring>(pickle());
		});
		return *this;
	}

	/// Writes the given message.
	///
	/// \param[in] thing The thing to write. This must be implicitly convertible
	///                  to a std::wstring using lexical_cast.
	template<
		typename TPrimitive,
		typename = std::enable_if_t<boost::is_fundamental<TPrimitive>::value>>
	LogMessage& operator<<(const TPrimitive& thing) {
		components.emplace_back([&thing]() {
			return boost::lexical_cast<std::wstring>(thing);
		});
		return *this;
	}

	/// Writes the given message.
	///
	/// \param[in] string The message to append. The message is not copied and
	///                   must have a lifetime longer than this.
	LogMessage& operator<<(const wchar_t* const string) {
		components.emplace_back([string] { return std::wstring(string); });
		return *this;
	}

	/// Writes the given message.
	///
	/// \param[in] string The message to append. The message is not copied and
	///                   must have a lifetime longer than this.
	LogMessage& operator<<(const char* const string) {
		components.emplace_back([string] {
			std::wstring result;
			result.resize(strlen(string));

			size_t resultCount = result.size();
			mbstowcs_s(&resultCount, &result.front(), resultCount, string,
				_TRUNCATE);

			result.erase(result.begin() + resultCount, result.end());
			return result;
		});
		return *this;
	}

	/// Writes the given message.
	///
	/// \param[in] thing The message to append. The message is not copied and
	///                  must have a lifetime longer than this.
	LogMessage& operator<<(const std::wstring& string) {
		components.emplace_back([&string] { return string; });
		return *this;
	}

	/// Writes the given message.
	///
	/// \param[in] thing The message to append. The message is not copied and
	///                  must have a lifetime longer than this.
	LogMessage& operator<<(const std::string& string) {
		components.emplace_back([&string] {
			std::wstring result;
			result.resize(string.length());

			size_t resultCount = result.size();
			mbstowcs_s(&resultCount, &result.front(), resultCount,
				string.c_str(), _TRUNCATE);

			result.erase(result.begin() + resultCount, result.end());
			return result;
		});
		return *this;
	}

	/// Compatibility with ostream manipulators. This is a no-op.
	LogMessage& operator<<(LogMessage& (*)(LogMessage&)) {
		return *this;
	}

private:
	/// Constructs a new log message buffer, with the given category.
	///
	/// \param[in] log The logger this message will write to.
	/// \param[in] category The category of the message we are creating.
	explicit LogMessage(Logger& log, std::wstring category = std::wstring());
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

namespace std {
	/// Technically I'm not allowed to do this by the C++ Standard. But this is
	/// needed for boost::spirit to work with our logger.
	You::Utils::LogMessage& endl(You::Utils::LogMessage& message) {
		return message;
	}
}  // namespace std

#endif  // YOU_UTILS_LOG_LOG_MESSAGE_H_

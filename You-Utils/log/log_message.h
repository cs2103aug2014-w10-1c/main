//@author A0097630B
#pragma once
#ifndef YOU_UTILS_LOG_LOG_MESSAGE_H_
#define YOU_UTILS_LOG_LOG_MESSAGE_H_

#include <string>
#include <vector>
#include <functional>
#include <type_traits>

namespace You {
namespace Utils {

/// This Logger class deals with logging one message. It is the actual class
/// that constructs log messages.
class LogMessage {
	friend class Logger;

	/// Template metaprogram to check if T is a functor.
	///
	/// Modified from http://stackoverflow.com/questions/15393938
	///
	/// \tparam TWithCVOrRef The raw type to check. This can have const/volatile
	///                      qualifiers (hence CV) or a reference (Ref).
	template<typename TWithCVOrRef>
	struct is_callable {
	private:
		template<bool array>
		struct check_callable_if_not_array {
		private:
			typedef char(&yes)[1];
			typedef char(&no)[2];

			struct Fallback { void operator()(); };
			#pragma warning(push)
			#pragma warning(disable: 4510 4610)
			struct Derived : T, Fallback {};
			#pragma warning(pop)

			template<typename U, U> struct Check;

			template<typename>
			static yes test(...);

			template<typename C>
			static no test(Check<void (Fallback::*)(), &C::operator()>*);

		public:
			static const bool value = sizeof(test<Derived>(0)) == sizeof(yes);
		};

		template<>
		struct check_callable_if_not_array<true> {
			static const bool value = false;
		};

		template<bool fundamental>
		struct check_callable_if_not_fundamental {
			static const bool value =
				check_callable_if_not_array<std::is_array<T>::value>::
					value;
		};
		
		template<>
		struct check_callable_if_not_fundamental<true> {
			static const bool value = false;
		};

		typename typedef std::remove_cv<TWithCVOrRef>::type TWithRef;
		typename typedef std::remove_reference<TWithRef>::type T;

	public:
		static const bool value =
			check_callable_if_not_fundamental<
				std::is_fundamental<T>::value>::value;
	};

	template<bool callable>
	struct writeTag {};
	typedef writeTag<true> callable;
	typedef writeTag<false> value;

public:
	/// Move constructor.
	LogMessage(LogMessage&& rhs);

	/// Destructor. Commits the message to the owning logger.
	~LogMessage();

	/// Writes the given message.
	///
	/// \param[in] thing The thing to write. This must be implicitly convertible
	///                  to a std::wstring using lexical_cast.
	template<typename TObject>
	LogMessage& operator<<(TObject&& object) {
		write(std::forward<TObject>(object),
			writeTag<is_callable<TObject>::value>());
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

	/// Writes the given message.
	///
	/// \param[in] pickle The function to apply. This must be a nullary function
	///                   returning a value convertible to std::wstring using
	///                   lexical_cast.
	template<typename TLazy>
	void write(const TLazy& pickle, callable) {
		components.emplace_back([pickle]() {
			return boost::lexical_cast<std::wstring>(pickle());
		});
	}

	/// Writes the given message.
	///
	/// \param[in] thing The thing to write. This must be implicitly convertible
	///                  to a std::wstring using lexical_cast.
	template<typename TObject>
	void write(const TObject& thing, value) {
		components.emplace_back([&thing]() {
			return boost::lexical_cast<std::wstring>(thing);
		});
	}

	/// Writes the given message.
	///
	/// \param[in] string The message to append. The message is not copied and
	///                   must have a lifetime longer than this.
	void write(const wchar_t* const string, value) {
		components.emplace_back([string] { return std::wstring(string); });
	}

	/// Writes the given message.
	///
	/// \param[in] string The message to append. The message is not copied and
	///                   must have a lifetime longer than this.
	void write(const char* const string, value) {
		components.emplace_back([string] {
			return LogMessage::toWString(string, strlen(string));
		});
	}

	/// Writes the given message.
	///
	/// \param[in] thing The message to append. The message is not copied and
	///                  must have a lifetime longer than this.
	void write(const std::wstring& string, value) {
		components.emplace_back([&string] { return string; });
	}

	/// Writes the given message.
	///
	/// \param[in] thing The message to append. The message is not copied and
	///                  must have a lifetime longer than this.
	void write(const std::string& string, value) {
		components.emplace_back([&string] {
			return LogMessage::toWString(string.c_str(), string.length());
		});
	}

	/// Compatibility with ostream manipulators. This is a no-op.
	LogMessage& operator<<(LogMessage& (*)(LogMessage&)) {
		return *this;
	}

	/// Helper to convert char strings to wchar_t strings.
	///
	/// \param[in] string The string to convert.
	/// \param[in] count The number of characters to convert.
	static std::wstring toWString(const char* string, size_t count);

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
inline You::Utils::LogMessage& endl(You::Utils::LogMessage& message) {
	return message;
}
}  // namespace std

#endif  // YOU_UTILS_LOG_LOG_MESSAGE_H_

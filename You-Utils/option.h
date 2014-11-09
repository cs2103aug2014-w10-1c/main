//@author A0097630B
#pragma once
#ifndef YOU_UTILS_OPTION_H_
#define YOU_UTILS_OPTION_H_

#include <boost/optional.hpp>

namespace You {
namespace Utils {

/// Implements an option type. This means that either the null value or an
/// actual value is contained within.
///
/// To test whether there is an actual value, cast this to a boolean.
template<typename T>
class Option : public boost::optional<T> {
public:
	/// Default constructor. This initialises a None value.
	inline Option() {
	}

	/// Copy construct from boost::none
	inline Option(const boost::none_t& value)  // NOLINT(runtime/explicit)
		: boost::optional<T>(value) {
	}

	/// Move constructor. This initialises a value from an existing value.
	inline Option(T&& value)  // NOLINT(runtime/explicit)
	: boost::optional<T>(std::move(value)) {
	}

	/// Copy constructor. This copies the value from an existing value.
	inline Option(const T& value)  // NOLINT(runtime/explicit)
	: boost::optional<T>(value) {
	}

	/// Move constructor.
	inline Option(const boost::optional<T>&& value)  // NOLINT(runtime/explicit)
 	: boost::optional<T>(value) {
	}

	/// By default boost::optional doesn't support the use of an implicit
	/// conversion to bool. We want this, so define our override.
	inline operator bool() const throw() {
		return boost::optional<T>::operator bool();
	}
};

template<typename T, typename... ParamTypes>
inline Option<T> make_option(ParamTypes&&... arguments) {
	return T(std::forward<ParamTypes>(arguments)...);
}

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_OPTION_H_

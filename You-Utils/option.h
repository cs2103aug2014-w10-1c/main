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

	/// Move constructor. This initialises a value from an existing value.
	inline Option(T&& value)  // NOLINT(runtime/explicit)
	: boost::optional<T>(std::move(value)) {
	}

	/// Copy constructor. This copies the value from an existing value.
	inline Option(const T& value)  // NOLINT(runtime/explicit)
	: boost::optional<T>(value) {
	}
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_OPTION_H_

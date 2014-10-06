//@author A0097630B
#pragma once
#ifndef YOU_UTILS_OPTION_H_
#define YOU_UTILS_OPTION_H_

#include <memory>
#include <cassert>

namespace You {
namespace Utils {

/// Implements an option type. This means that either the null value or an
/// actual value is contained within.
///
/// To test whether there is an actual value, cast this to a boolean.
template<typename T>
class Option : protected std::shared_ptr<T> {
public:
	/// Default constructor. This initialises a None value.
	inline Option() {
	}

	/// Move constructor. This initialises a value from an existing value.
	inline Option(T&& value)  // NOLINT(runtime/explicit)
	: std::shared_ptr<T>(new T(value)) {
	}

	/// Copy constructor. This copies the value from an existing value.
	inline Option(const T& value)  // NOLINT(runtime/explicit)
	: std::shared_ptr<T>(new T(value)) {
	}

	/// Conversion to bool. This is to check if this is a None value.
	inline operator bool() const {
		return std::shared_ptr<T>::get() != nullptr;
	}

	/// Retrieves the value of the option type. The result is undefined if this
	/// is a None value.
	inline const T& get() const {
		assert(operator bool());
		return *std::shared_ptr<T>::get();
	}

	/// Retrieves the value of the option type. The result is undefined if this
	/// is a None value.
	inline T& get() {
		assert(operator bool());
		return *std::shared_ptr<T>::get();
	}

	/// Checks for equality with another Option value. Equality is defined as
	/// none-ness, and if both are not none, then equality of the value.
	inline bool operator==(const Option<T>& rhs) const {
		bool leftIsNull = !operator bool();
		bool rightIsNull = !rhs.operator bool();

		if (leftIsNull == rightIsNull) {
			return leftIsNull || get() == rhs.get();
		} else {
			return false;
		}
	}

	/// Inverse of \ref Option<T>::operator==
	inline bool operator!=(const Option<T>& rhs) const {
		bool leftIsNull = !operator bool();
		bool rightIsNull = !rhs.operator bool();

		if (leftIsNull == rightIsNull) {
			return !leftIsNull && get() != rhs.get();
		} else {
			return leftIsNull != rightIsNull;
		}
	}
};

}  // namespace Utils
}  // namespace You

#endif  // YOU_UTILS_OPTION_H_

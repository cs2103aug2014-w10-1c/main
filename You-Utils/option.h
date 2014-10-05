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
	inline Option() {
	}

	inline Option(T&& value)
	: std::shared_ptr<T>(new T(value)) {
	}

	inline Option(const T& value)
	: std::shared_ptr<T>(new T(value)) {
	}

	inline operator bool() const {
		return std::shared_ptr<T>::get() != nullptr;
	}

	inline const T& get() const {
		assert(operator bool());
		return *std::shared_ptr<T>::get();
	}

	inline T& get() {
		assert(operator bool());
		return *std::shared_ptr<T>::get();
	}

	inline bool operator==(const Option<T>& rhs) const {
		bool leftIsNull = !operator bool();
		bool rightIsNull = !rhs.operator bool();

		if (leftIsNull == rightIsNull) {
			return leftIsNull || get() == rhs.get();
		} else {
			return false;
		}
	}

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

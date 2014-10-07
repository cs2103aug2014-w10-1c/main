//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_EXCEPTIONS_CONTEXT_INDEX_OUT_OF_RANGE_EXCEPTION_H_
#define YOU_CONTROLLER_EXCEPTIONS_CONTEXT_INDEX_OUT_OF_RANGE_EXCEPTION_H_

#include "exception.h"

namespace You {
namespace Controller {

/// A context index out of range exception. This can be caught using the You
/// exception base class or std::out_of_range.
class ContextIndexOutOfRangeException :
	public virtual Exception,
	public virtual std::out_of_range {
public:
	/// Creates a new exception.
	inline ContextIndexOutOfRangeException()
	: std::out_of_range(nullptr) {
	}

	/// Constructs the exception from an existing std::out_of_range
	///
	/// \param[in] exception The exception to construct the new exception from.
	inline ContextIndexOutOfRangeException(const std::out_of_range& exception)
	: std::out_of_range(exception) {
	}

	/// Copy constructor.
	inline ContextIndexOutOfRangeException(
		const ContextIndexOutOfRangeException &exception)
	: Exception(exception), std::out_of_range(exception) {
	}
};

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_EXCEPTIONS_CONTEXT_INDEX_OUT_OF_RANGE_EXCEPTION_H_

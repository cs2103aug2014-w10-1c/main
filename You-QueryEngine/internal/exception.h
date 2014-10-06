/// \file You-QueryEngine/internal/exception.h
/// Defines some exception classes.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_
#define YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_

#include <stdexcept>
#include "../../You-Utils/exceptions/exception.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Base exception class.
class Exception : public You::Utils::Exception {
};

/// An exception thrown when trying to create task
/// with empty description.
class EmptyTaskDescriptionException : public Exception {
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_

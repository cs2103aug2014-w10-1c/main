/// \file exception.h
/// Defines some exception classes.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_
#define YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_

#include <stdexcept>

namespace You {
namespace QueryEngine {
namespace Internal {

class Exception : public std::exception {
};

class EmptyTaskDescriptionException : public Exception {
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_EXCEPTION_H_

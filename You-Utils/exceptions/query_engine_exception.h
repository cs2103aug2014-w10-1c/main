/// \file You-QueryEngine/internal/exception.h
/// Defines some exception classes.
/// \author A0112054Y

#pragma once
#ifndef YOU_UTILS_EXCEPTIONS_QUERY_ENGINE_EXCEPTION_H_
#define YOU_UTILS_EXCEPTIONS_QUERY_ENGINE_EXCEPTION_H_

#include <stdexcept>
#include "exception.h"

namespace You {
namespace QueryEngine {
namespace Exception {

/// Base exception class.
class BaseException : public You::Utils::Exception {
};

/// An exception thrown when trying to create task
/// with empty description.
class EmptyTaskDescriptionException : public BaseException {
};

/// An exception thrown when trying to delete or edit a task
/// that does not exists.
class TaskNotFoundException : public BaseException {
};

/// An exception thrown when dependency of two or more tasks
/// is circular
class CircularDependencyException : public BaseException {
};

/// An exception thrown when trying to undo an undoable
/// query (e.g GetTask)
class NotUndoAbleException : public BaseException {
};

}  // namespace Exception
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_UTILS_EXCEPTIONS_QUERY_ENGINE_EXCEPTION_H_

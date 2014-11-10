/// \file You-QueryEngine/exception.h
/// Defines some exception classes.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_EXCEPTION_H_
#define YOU_QUERYENGINE_EXCEPTION_H_

#include "You-Utils/exceptions/exception.h"

namespace You {
namespace QueryEngine {
namespace Exception {

/// Base exception class that will be thrown by Query Engine.
class BaseException : public You::Utils::Exception {
};

/// An exception thrown when trying to create task with empty description.
class EmptyTaskDescriptionException : public BaseException {
};

/// An exception thrown when trying to delete or edit a task that does not
/// exists.
class TaskNotFoundException : public BaseException {
};

/// An exception thrown when dependency/subtasks of two or more tasks
/// is circular.
class CircularDependencyException : public BaseException {
};

/// An exception thrown when trying to undo an undoable query (e.g GetTask)
class NotUndoAbleException : public BaseException {
};

/// An exception thrown when trying to attach a task that
/// already has a parent to another task.
class TaskAlreadyHasParentException : public BaseException {
};

}  // namespace Exception
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_EXCEPTION_H_

/// \file schema.h
/// Defines several query schema.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_SCHEMA_H_
#define YOU_QUERYENGINE_SCHEMA_H_

#include <string>
#include "task_model.h"

namespace You {
namespace QueryEngine {
namespace Query {

/// Schema for AddTask command.
struct AddTask {
	Task::Description description;  ///< Description.
	Task::Time deadline;  ///< Deadline.
	Task::Priority priority;  ///< Priority.
	Task::Dependencies dependencies;  ///< Dependencies.
};

/// Schema for GetTask command.
struct GetTask {
	Task::ID taskID;  ///< ID
};

/// Schema for DeleteTask command.
struct DeleteTask {
	Task::ID taskID;  ///< ID
};

/// Schema for FindTask command.
struct FindTask {
	std::wstring searchKeyword;  ///< Keyword
};

/// Schema for EditDeadline command.
struct EditDeadline {
	Task::ID taskID;  ///< ID
	Task::Time newDeadline;  ///< Deadline
};

/// Schema for EditDescription command.
struct EditDescription {
	Task::ID taskID;  ///< ID
	Task::Description newDescription;  ///< Description
};

/// Schema for AddDependency command.
struct AddDependency {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
};

/// Schema for RemoveDependency command.
struct RemoveDependency {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
};

/// Schema for Undo command.
struct Undo {
};

/// Schema for Sort command.
struct Sort {
	std::wstring comparatorField;  ///< Comparator field
};

}  // namespace Query
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_SCHEMA_H_

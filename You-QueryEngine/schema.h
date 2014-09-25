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

/// Base class for queries
struct Any {};

/// Schema for AddTask command.
struct AddTask : public Any {
	Task::Description description;  ///< Description.
	Task::Time deadline;  ///< Deadline.
	Task::Priority priority;  ///< Priority.
	Task::Dependencies dependencies;  ///< Dependencies.
};

/// Schema for GetTask command.
struct GetTask : public Any {
	Task::ID taskID;  ///< ID
};

/// Schema for DeleteTask command.
struct DeleteTask : public Any {
	Task::ID taskID;  ///< ID
};

/// Schema for FindTask command.
struct FindTask : public Any {
	std::wstring searchKeyword;  ///< Keyword
};

/// Schema for EditDeadline command.
struct EditDeadline : public Any {
	Task::ID taskID;  ///< ID
	Task::Time newDeadline;  ///< Deadline
};

/// Schema for EditDescription command.
struct EditDescription : public Any {
	Task::ID taskID;  ///< ID
	Task::Description newDescription;  ///< Description
};

/// Schema for AddDependency command.
struct AddDependency : public Any {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
};

/// Schema for RemoveDependency command.
struct RemoveDependency : public Any {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
};

/// Schema for Undo command.
struct Undo : public Any {
};

/// Schema for Sort command.
struct Sort : public Any {
	std::wstring comparatorField;  ///< Comparator field
};

}  // namespace Query
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_SCHEMA_H_

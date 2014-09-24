/// \file schema.h
/// Defines several query schema.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_SCHEMA_H_
#define YOU_QUERYENGINE_INTERNAL_SCHEMA_H_

#include <string>
#include "task_model.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Defines schema for raw queries from NLP parser that will be
/// converted to real queries, executed, and converted back to string
/// to be displayed in the GUI.
namespace Query {
struct Query {};

/// Schema for AddTask command.
struct AddTask : public Query {
	Task::Description description;  ///< Description.
	Task::Time deadline;  ///< Deadline.
	Task::Priority priority;  ///< Priority.
	Task::Dependencies dependencies;  ///< Dependencies.
};

/// Schema for GetTask command.
struct GetTask : public Query {
	Task::ID taskID;  ///< ID
};

/// Schema for DeleteTask command.
struct DeleteTask : public Query {
	Task::ID taskID;  ///< ID
};

/// Schema for FindTask command.
struct FindTask : public Query {
	std::wstring searchKeyword;  ///< Keyword
};

/// Schema for EditDeadline command.
struct EditDeadline : public Query {
	Task::ID taskID;  ///< ID
	Task::Time newDeadline;  ///< Deadline
};

/// Schema for EditDescription command.
struct EditDescription : public Query {
	Task::ID taskID;  ///< ID
	Task::Description newDescription;  ///< Description
};

/// Schema for AddDependency command.
struct AddDependency : public Query {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
};

/// Schema for RemoveDependency command.
struct RemoveDependency : public Query {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
};

/// Schema for Undo command.
struct Undo : public Query {
};

/// Schema for Sort command.
struct Sort : public Query {
	std::wstring comparatorField;  ///< \example "deadline"
};

}  // namespace Query
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_SCHEMA_H_

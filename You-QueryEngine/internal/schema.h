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
	/// Explicit Constructor
	explicit AddTask(Task::Description description,
		Task::Time deadline, Task::Priority priority,
		Task::Dependencies dependencies) :
		description(description),
		deadline(deadline),
		priority(priority),
		dependencies(dependencies)
	{}
};

/// Schema for GetTask command.
struct GetTask : public Query {
	Task::ID taskID;  ///< ID
	/// Constructor
	explicit GetTask(Task::ID id)
		: taskID(id) {}
};

/// Schema for DeleteTask command.
struct DeleteTask : public Query {
	Task::ID taskID;  ///< ID
	/// Constructor
	explicit DeleteTask(Task::ID id)
		: taskID(id) {}
};

/// Schema for FindTask command.
struct FindTask : public Query {
	std::wstring searchKeyword;  ///< Keyword
	/// Constructor
	explicit FindTask(std::wstring searchKeyword)
		: searchKeyword(searchKeyword) {}
};

/// Schema for EditDeadline command.
struct EditDeadline : public Query {
	Task::ID taskID;  ///< ID
	Task::Time newDeadline;  ///< Deadline
	/// Constructor
	explicit EditDeadline(Task::ID taskID, Task::Time newDeadline)
		: taskID(taskID), newDeadline(newDeadline) {}
};

/// Schema for EditDescription command.
struct EditDescription : public Query {
	Task::ID taskID;  ///< ID
	Task::Description newDescription;  ///< Description
	/// Constructor
	explicit EditDescription(Task::ID taskID, Task::Description newDescription)
		: taskID(taskID), newDescription(newDescription) {}
};

/// Schema for AddDependency command.
struct AddDependency : public Query {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
	/// Constructor
	explicit AddDependency(Task::ID taskID, Task::ID dependencyID)
		: taskID(taskID), dependencyID(dependencyID) {}
};

/// Schema for RemoveDependency command.
struct RemoveDependency : public Query {
	Task::ID taskID;  ///< ID
	Task::ID dependencyID;  ///< ID
	/// Constructor
	explicit RemoveDependency(Task::ID taskID, Task::ID dependencyID)
		: taskID(taskID), dependencyID(dependencyID) {}
};

/// Schema for Undo command.
struct Undo : public Query {
};

/// Schema for Sort command.
struct Sort : public Query {
	std::wstring comparatorField;  ///< \example "deadline"
	explicit Sort(std::wstring comparatorField)
		: comparatorField(comparatorField) {}
};

}  // namespace Query
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_SCHEMA_H_

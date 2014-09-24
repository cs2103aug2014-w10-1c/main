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
	std::wstring description;  /// Description.
	std::wstring deadline;  /// Deadline.
	std::wstring priority;  /// Priority.
	std::wstring dependencies;  /// Dependencies.
};

/// Schema for GetTask command.
struct GetTask : public Query {
	std::wstring taskID;  /// ID
};

/// Schema for DeleteTask command.
struct DeleteTask : public Query {
	std::wstring taskID;  /// ID
};

/// Schema for FindTask command.
struct FindTask : public Query {
	std::wstring searchKeyword;  /// Keyword
};

/// Schema for EditDeadline command.
struct EditDeadline : public Query {
	std::wstring taskID;  /// ID
	std::wstring newDeadline;  /// Deadline
};

/// Schema for EditDescription command.
struct EditDescription : public Query {
	std::wstring taskID;  /// ID
	std::wstring newDescription;  /// Description
};

/// Schema for AddDependency command.
struct AddDependency : public Query {
	std::wstring taskID;  /// ID
	std::wstring dependencyID;  /// ID
};

/// Schema for RemoveDependency command.
struct RemoveDependency : public Query {
	std::wstring taskID;  /// ID
	std::wstring dependencyID;  /// ID
};

/// Schema for Undo command.
struct Undo : public Query {
};

/// Schema for Sort command.
struct Sort : public Query {
	std::wstring comparatorField;  /// \example "deadline"
};

}  // namespace Query
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_SCHEMA_H_

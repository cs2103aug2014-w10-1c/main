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

struct AddTask : public Query {
	std::wstring description;
	std::wstring deadline;
	std::wstring priority;
	std::wstring dependencies;
};

struct GetTask : public Query {
	std::wstring taskID;
};

struct DeleteTask : public Query {
	std::wstring taskID;
};

struct FindTask : public Query {
	std::wstring searchKeyword;
};

struct EditDeadline : public Query {
	std::wstring taskID;
	std::wstring newDeadline;
};

struct EditDescription : public Query {
	std::wstring taskID;
	std::wstring newDescription;
};

struct AddDependency : public Query {
	std::wstring taskID;
	std::wstring dependencyID;
};

struct RemoveDependency : public Query {
	std::wstring taskID;
	std::wstring dependencyID;
};

struct Undo : public Query {
};

struct Sort : public Query {
	std::wstring comparatorField;
};

}  // namespace Query
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_SCHEMA_H_

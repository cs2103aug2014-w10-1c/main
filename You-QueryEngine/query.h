/// \file query.h
/// Defines the API for Query Engine.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_QUERY_H_
#define YOU_QUERYENGINE_QUERY_H_

#include <memory>
#include <boost/variant.hpp>
#include "internal/schema.h"
#include "internal/task_model.h"

namespace You {
namespace QueryEngine {

typedef Internal::Query::Query Query;
typedef Internal::Task Task;

/// Return a query for adding a task
/// \note Please use Task::DEFAULT_xxx to replace incomplete fields.
std::unique_ptr<Query> AddTask(
	  Task::Description description
	, Task::Time deadline
	, Task::Priority priority
	, Task::Dependencies dependencies
);

/// Return a query for fetching single task.
std::unique_ptr<Query> GetTask(Task::ID id);

/// Return a query for deleting single task.
std::unique_ptr<Query> DeleteTask(Task::ID taskID);

/// Return a query for searching task based on keyword.
/// \todo Implement custom filters using LINQ-like syntax
std::unique_ptr<Query> FindTask(std::wstring searchKeyword);

/// Return a query for set new deadline for a task.
std::unique_ptr<Query> EditDeadline(Task::ID taskID,
							        Task::Time newDeadline);

/// Return a query for set new description for a task.
std::unique_ptr<Query> EditDescription(Task::ID taskID,
									   Task::Time newDescription);

/// Return a query for adding new dependency for a task.
std::unique_ptr<Query> AddDependency(Task::ID taskID, Task::ID dependencyID);

/// Return a query for removing dependency from a task.
std::unique_ptr<Query> RemoveDependency(Task::ID taskID, Task::ID dependencyID);

/// Return a query for undoing actions.
std::unique_ptr<Query> Undo();

/// Return a query for sorting a list of tasks based on comparator.
/// \todo Implement usable comparators instead of string.
std::unique_ptr<Query> Sort(std::vector<Task::ID> tasks,
	                        std::wstring comparator);

typedef boost::variant <
	  std::vector<Task>
	, Task
	, Task::ID
	, Task::Time
	, Task::Dependencies
	, Task::Description
	> Response;

/// Execute a query from the parser.
Response executeQuery(std::unique_ptr<Query> query);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_QUERY_H_

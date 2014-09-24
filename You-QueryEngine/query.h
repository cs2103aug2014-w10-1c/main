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

std::shared_ptr<Query> AddTask(
	  Task::Description description
	, Task::Time deadline
	, Task::Priority priority
	, Task::Dependencies dependencies
);

std::shared_ptr<Query> GetTask(Task::ID id);

std::shared_ptr<Query> DeleteTask(Task::ID taskID);

std::shared_ptr<Query> FindTask(std::wstring searchKeyword);

std::shared_ptr<Query> EditDeadline(Task::ID taskID, Task::Time newDeadline);

std::shared_ptr<Query> EditDescription(Task::ID taskID, Task::Time newDescription);

std::shared_ptr<Query> AddDependency(Task::ID taskID, Task::ID dependencyID);

std::shared_ptr<Query> RemoveDependency(Task::ID taskID, Task::ID dependencyID);

std::shared_ptr<Query> Undo();

std::shared_ptr<Query> Sort(std::wstring comparator);

boost::variant <
	  std::vector<Task>
	, Task
	, Task::ID
	, Task::Time
	, Task::Dependencies
	, Task::Description
> Response;

/// Execute a query from the parser.
Response executeQuery(std::shared_ptr<Query> query);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_QUERY_H_

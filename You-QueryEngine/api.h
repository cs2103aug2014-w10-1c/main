/// \file api.h
/// Defines the API for Query Engine.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_API_H_
#define YOU_QUERYENGINE_API_H_

#include <memory>
#include <boost/variant.hpp>
#include "task_model.h"
#include "filter.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class QueryEngineTests; }
namespace Internal { class State; }

/// A synthesized type for holding query responses
typedef boost::variant<std::vector<Task>, Task,
	Task::ID, std::wstring> Response;

/// Base class for all queries.
class Query {
	friend Response executeQuery(std::unique_ptr<Query> query);
private:
	/// Execute the query.
	virtual Response execute(Internal::State& tasks) = 0;
};

/// \name Query Constructors
/// @{
/// Construct a query for adding a task
/// \note Please use Task::DEFAULT_xxx to replace incomplete fields.
std::unique_ptr<Query>
AddTask(Task::Description description, Task::Time deadline,
Task::Priority priority, Task::Dependencies dependencies);

std::unique_ptr<Query>
FilterTask(const Filter& filter);

std::unique_ptr<Query>
DeleteTask(Task::ID id);

std::unique_ptr<Query>
UpdateTask(Task::ID id, Task::Description description,
Task::Time deadline, Task::Priority priority, Task::Dependencies dependencies);
/// @}

/// Execute a query and return a response
///  \return The result of the query as a response object.
///  \deprecated
Response executeQuery(std::unique_ptr<Query> query);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_API_H_

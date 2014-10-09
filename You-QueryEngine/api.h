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
	friend class QueryEngine;
private:
	/// Execute the query on a state.
	virtual Response execute(Internal::State& state) = 0;
};

/// Utility class for QueryEngine
/// This is the API which will be called by Controller
class QueryEngine {
public:
	/// \name All types used in this component.
	/// @{
	typedef You::QueryEngine::Task Task;
	typedef You::QueryEngine::Query Query;
	typedef You::QueryEngine::Filter Filter;
	typedef You::QueryEngine::Response Response;
	/// @}

	#pragma region Query Constructors
	/// Construct add task query.
	static std::unique_ptr<Query> AddTask(Task::Description description,
		Task::Time deadline, Task::Priority priority,
		Task::Dependencies dependencies);

	/// Construct filter task query.
	static std::unique_ptr<Query> FilterTask(const Filter& filter);

	/// Construct delete task query.
	static std::unique_ptr<Query> DeleteTask(Task::ID id);

	/// Construct update task query.
	static std::unique_ptr<Query> UpdateTask(Task::ID id,
		Task::Description description, Task::Time deadline,
		Task::Priority priority, Task::Dependencies dependencies);

	/// Construct mark task as done query.
	static std::unique_ptr<Query> UpdateTask(Task::ID id, bool completed);
	#pragma endregion

	/// Execute a query and return a response
	///  \return The result of the query as a response object.
	static Response executeQuery(std::unique_ptr<Query> query);

private:
	QueryEngine() = delete;

};  // class QueryEngine

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_API_H_

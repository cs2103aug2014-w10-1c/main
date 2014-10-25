/// \file api.h
/// The entry point of the QueryEngine API, defines the QueryEngine
/// class and Query interface.\n
/// This component interfaces with You::Controller and
/// You::DataStore.\n

/// \name Namespaces
/// @{
/// \namespace You::QueryEngine
/// Responsible mainly for implementing and accepting
/// high level queries from NLP and forward it to
/// DataStore.
/// \namespace You::QueryEngine::Internal
/// Hidden internal components.
/// \namespace You::QueryEngine::Internal::Action
/// Several classes that implements Query actions.
/// \namespace You::QueryEngine::UnitTests
/// Contains the test classes for QueryEngine.
/// @}

/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_API_H_
#define YOU_QUERYENGINE_API_H_

#include <memory>
#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include "task_model.h"
#include "filter.h"
#include "comparator.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class QueryEngineTests; }
namespace Internal { namespace Action { class Undo; } class State; }

/// A synthesized type for holding query responses
typedef boost::variant<std::vector<Task>, Task,
	Task::ID, std::wstring> Response;

/// Interface for queries.
/// Implement execute method which takes a State
/// as a parameter and return a Response\n
class Query {
	friend class QueryEngine;
	friend class Internal::Action::Undo;

protected:
	/// Get the reverse of this query for undo.
	virtual std::unique_ptr<Query> getReverse();

	/// String appended before each log message.
	static const std::wstring logCategory;

private:
	/// Execute the query on a state.
	/// \pre The state has been loaded and valid.
	virtual Response execute(Internal::State& state) = 0;
};

/// Utility class for QueryEngine \n
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

public:
	#pragma region Query Constructors
	/// Construct add task query.
	static std::unique_ptr<Query> AddTask(Task::Description description,
		Task::Time deadline, Task::Priority priority,
		Task::Dependencies dependencies);

	/// Construct filter task without sort query
	static std::unique_ptr<Query> GetTask(const Filter& filter);

	/// Construct filter and sort task query.
	static std::unique_ptr<Query> GetTask(const Filter& filter,
		const Comparator& comparator);

	/// Construct delete task query.
	static std::unique_ptr<Query> DeleteTask(Task::ID id);

	/// Construct update task query.
	static std::unique_ptr<Query> UpdateTask(Task::ID id,
		boost::optional<Task::Description> description,
		boost::optional<Task::Time> deadline,
		boost::optional<Task::Priority> priority,
		boost::optional<Task::Dependencies> dependencies,
		boost::optional<bool> completed);

	/// Construct undo query.
	static std::unique_ptr<Query> Undo();

	#pragma endregion

public:
	/// Execute a query and return a response
	///  \return The result of the query as a response object.
	static Response executeQuery(std::unique_ptr<Query> query);

private:
	QueryEngine() = delete;
};  // class QueryEngine

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_API_H_

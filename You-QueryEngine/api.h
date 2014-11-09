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

//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_API_H_
#define YOU_QUERYENGINE_API_H_

#include <memory>
#include <boost/variant.hpp>
#include "../You-Utils/option.h"
#include "task_model.h"
#include "filter.h"
#include "comparator.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class QueryEngineTests; }
namespace Internal { class State; }

namespace {
	using You::Utils::Option;
}  // namespace

/// A synthesized type for holding query responses
typedef boost::variant<std::vector<Task>, Task,
	Task::ID, std::wstring> Response;

/// Interface for queries.
/// Implement execute method which takes a State
/// as a parameter and return a Response\n
class Query {
	friend class QueryEngine;
public:
	/// Execute the query on a state.
	/// \pre The state has been loaded and valid.
	virtual Response execute(Internal::State& state) = 0;

protected:
	/// Get the reverse of this query for undo.
	virtual std::unique_ptr<Query> getReverse();

	/// String appended before each log message.
	static const std::wstring logCategory;
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
	/// Delta data structure to denote insertion/deletion of elements.
	/// This will be used and passed to UpdateTask by the Controller.
	template<typename T>
	struct Delta {
		enum class Type { ADD, DELETE };
		/// The type of the delta.
		Type type;
		/// The element.
		std::vector<T> elements;
	};

public:
	/// Construct the query for adding tasks.
	static std::unique_ptr<Query> AddTask(
		const Task::Description& description,
		const Task::Time& startTime,
		const Task::Time& deadline,
		const Task::Priority& priority,
		std::vector<std::unique_ptr<Query>>&& dependencies,
		std::vector<std::unique_ptr<Query>>&& subtasks);

	/// Reapply current filter and sorter to the task lists.
	static std::unique_ptr<Query> GetTask();

	/// Construct filter task without sort query.
	static std::unique_ptr<Query> GetTask(const Filter& filter);

	/// Construct filter and sort task query.
	static std::unique_ptr<Query> GetTask(const Filter& filter,
		const Comparator& comparator);

	/// Construct delete task query.
	static std::unique_ptr<Query> DeleteTask(Task::ID id);

	/// Construct update task query.
	static std::unique_ptr<Query> UpdateTask(Task::ID id,
		Option<Task::Description> description,
		Option<Task::Time> startTime,
		Option<Task::Time> deadline,
		Option<Task::Priority> priority,
		Delta<Task::Dependencies::value_type> dependencies,
		Option<bool> completed,
		Option<Task::ID> parent,
		Delta<Task::Subtasks::value_type> subtasks,
		Delta<Task::Attachment::value_type> attachment);

public:
	/// Construct adding dependency query.
	static std::unique_ptr<Query> AddDependency(Task::ID id,
		Task::ID dependency);

	/// Construct  removind dependency query.
	static std::unique_ptr<Query> RemoveDependency(Task::ID id,
		Task::ID dependency);

	/// Construct add subtask query.
	static std::unique_ptr<Query> AddSubtask(Task::ID id,
		Task::ID subtask);

	/// Construct remove subtask query.
	static std::unique_ptr<Query> RemoveSubtask(Task::ID id,
		Task::ID subtask);

	/// Shortest version of update query.
	static std::unique_ptr<Query> UpdateTask(const Task& task);

	/// Construct undo query.
	static std::unique_ptr<Query> Undo();

public:
	/// Execute a query and return a response
	///  \return The result of the query as a response object.
	static Response executeQuery(std::unique_ptr<Query> query);

private:
	static Option<Task::Attachment> attachmentsFromDelta(
		const Delta<Task::Attachment::value_type>& attachment,
		const Task& task);

	static Option<Task::Subtasks> subtasksFromDelta(
		const Delta<Task::Subtasks::value_type>& subtasks,
		const Task& task);

	static Option<Task::Dependencies> dependenciesFromDelta(
		const Delta<Task::Dependencies::value_type>& dependencies,
		const Task& task);

	QueryEngine() = delete;
};  // class QueryEngine

/// String representation of a task, for testing and logging.
///
/// \param[in] task The task object, assumed all fields are valid
/// \return A string representation of the task
std::wstring ToString(const Task& task);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_API_H_

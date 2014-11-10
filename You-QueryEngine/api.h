/// \file api.h
/// The entry point of the QueryEngine API.
/// It contains various Query constructor that will be called and
/// executed by the Controller.
///
/// \name Namespaces
/// \namespace You::QueryEngine
/// The exposed functionalities of the Query Engine.
/// \namespace You::QueryEngine::Exception
/// Contains various exceptions that may be thrown by Query Engine
/// \namespace You::QueryEngine::Internal
/// The hidden internal components of the Query Engine.
/// \namespace You::QueryEngine::Internal::Action
/// Contains various queries that is constructed via the API.
/// \namespace You::QueryEngine::UnitTests
/// Contains the unit test classes for QueryEngine components.
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

using You::Utils::Option;

/// A synthesized type for holding query responses
/// \code {cpp}
///		 auto deleteQuery = QueryEngine::DeleteTask(id);
///		 auto response = QueryEngine::executeQuery(std::move(deleteQuery));
///		 auto deletedId = boost::get<Task::ID>(response);
/// \endcode
typedef boost::variant<std::vector<Task>, Task, Task::ID> Response;

/// Interface for queries (or in our terms, actions).
/// Actions have execute method which modifies the state and return a response
/// object.
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
    /// The task model.
    typedef You::QueryEngine::Task Task;

    /// The query interface.
    typedef You::QueryEngine::Query Query;

    /// The filter used for filtering task.
    typedef You::QueryEngine::Filter Filter;

    /// The response type of the query action.
    typedef You::QueryEngine::Response Response;
    /// @}

public:
    /// Delta data structure to represent insertion/deletion of elements.
    /// This will be used and passed to the \ref UpdateTask method.
    template<typename T>
    struct Delta {
        /// Delta can be addition, deletion, or no change.
        enum class Type { ADD, DELETE, NO_CHANGE };

        /// Default constructor.
        Delta()
        : type(Type::NO_CHANGE), elements() {}

        /// Construct with type and elements.
        Delta(Type type, const std::vector<T>& elements)
        : type(type), elements(elements) {}

        /// The type of the delta.
        Type type;

        /// The element stored in the delta.
        std::vector<T> elements;
    };

public:
    /// Construct a query to add a task, possibly along with its dependencies
	/// and subtasks.
	/// \see \ref BatchAddDependencies
	/// \see \ref BatchAddSubtasks
	/// \param [in] description The description of the new task.
	/// \param [in] startTime The startTime of the new task.
	/// \param [in] deadline The deadline of the new task.
	/// \param [in] priority The priority of the new task.
	/// \param [in] dependencies The list of add queries for the dependencies.
	/// \param [in] subtasks The list of add queries for the subtasks.
	/// \return A query which, when executed will add a single parent task
	/// after executing the queries given in dependencies and subtasks.
    static std::unique_ptr<Query> AddTask(
        const Task::Description& description,
        const Task::Time& startTime,
        const Task::Time& deadline,
        const Task::Priority& priority,
        std::vector<std::unique_ptr<Query>>&& dependencies,
        std::vector<std::unique_ptr<Query>>&& subtasks);

	/// Construct query to get the list of tasks.
	/// \return A query which, when executed will get all task
	/// that match the last filter used, then sort it based on the
	/// last comparator used.
    static std::unique_ptr<Query> GetTask();

	/// Construct query to get the list of tasks.
	/// \param [in] filter The filter object to be used.
	/// \param [in] comparator The comparator object to be used.
	/// \return A query which, when executed will get all task
	/// that match the current filter, then sort it based on the
	/// creation time of the task.
    static std::unique_ptr<Query> GetTask(const Filter& filter);

	/// Construct query to get the list of tasks.
	/// \param [in] filter The filter object to be used.
	/// \param [in] comparator The comparator object to be used.
	/// \return A query which, when executed will get all task
	/// that match the current filter, then sort it using the
	/// comparator.
    static std::unique_ptr<Query> GetTask(const Filter& filter,
        const Comparator& comparator);

    /// Construct a query for deleting a task.
	/// \param [in] id The id of the task to be deleted.
	/// \return A query which, when executed will delete the task with
	/// specified id
    static std::unique_ptr<Query> DeleteTask(Task::ID id);

    /// Construct a query for updating a task.
	/// \see You::Utils:Option for usage of option type.
	/// \see Delta for usage of delta type.
	/// \param [in] id The id of the current task.
	/// \param [in] description The task description.
	/// \param [in] startTime The task starting time.
	/// \param [in] deadline The deadline.
	/// \param [in] priority The priority.
	/// \param [in] dependencies The delta of the dependencies.
	/// \param [in] completed The completion status.
	/// \param [in] parent The new parent of the task.
	/// \param [in] subtasks The delta of the subtasks.
	/// \param [in] attachment The delta of the attachment.
	/// \return A query which, when executed will update the current task
	/// according to the parameter given.
    static std::unique_ptr<Query> UpdateTask(
		Task::ID id,
        Option<Task::Description> description,
        Option<Task::Time> startTime,
        Option<Task::Time> deadline,
        Option<Task::Priority> priority,
        Delta<Task::Dependencies::value_type> dependencies,
        Option<bool> completed,
        Option<Task::ID> parent,
        Delta<Task::Subtasks::value_type> subtasks,
        Delta<Task::Attachment::value_type> attachment);


    /// Construct a query for updating a task.
	/// \param [in] task The task object to be updated.
	/// \return A query which, when executed, update the task.
    static std::unique_ptr<Query> UpdateTask(const Task& task);

public:
    /// Construct a query to add a task as a dependency of another task.
	/// \param [in] id The id of the depending task.
	/// \param [in] dependency The id of the dependent task.
	/// \return A query that, when executed, add the task as a dependency.
    static std::unique_ptr<Query> AddDependency(
		Task::ID id, Task::ID dependency);

    /// Construct a query to remove a task from the dependencies
	/// of another Task.
	/// \param [in] id The id of the depending task.
	/// \param [in] dependency The id of the task to be removed.
	/// \note This method is non-recursive.
	/// \return A query that, when executed, remove the task from
	/// the dependency.
    static std::unique_ptr<Query> RemoveDependency(
		Task::ID id, Task::ID dependency);

    /// Construct a query to add a task as a subtask of another task.
	/// \param [in] id The id of the parent task.
	/// \param [in] subtask The id of the subtask.
	/// \return A query that, when executed, add the task as a subtask.
    static std::unique_ptr<Query> AddSubtask(
		Task::ID id, Task::ID subtask);

    /// Construct a query to remove a task from a subtask of another task.
	/// \param [in] id The id of the parent task.
	/// \param [in] subtask The id of the subtask.
	/// \return A query that, when executed, remove the task from the subtasks.
    static std::unique_ptr<Query> RemoveSubtask(
		Task::ID id, Task::ID subtask);

    /// Construct undo query.
    static std::unique_ptr<Query> Undo();

public:
    /// Execute a query and return a \ref Response object.
    /// \param [in] query The query to be executed
    /// \return The result of the query as a response object.
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

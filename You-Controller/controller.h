//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_CONTROLLER_H_
#define YOU_CONTROLLER_CONTROLLER_H_

#include <string>
#include <unordered_map>
#include "result.h"

namespace You {
namespace NLP { enum class TaskPriority; }
namespace Controller {
namespace Internal {
class QueryExecutorBuilderVisitor;

namespace UnitTests { class QueryExecutorBuilderVisitorTests; }
}  // namespace Internal
namespace UnitTests { class ControllerContextTests; }

/// The entry-point for all communications with the rest of the You
/// framework.
///
/// This is a singleton class.
class Controller {
	friend class UnitTests::ControllerContextTests;
	friend class Internal::QueryExecutorBuilderVisitor;
	friend class Internal::UnitTests::QueryExecutorBuilderVisitorTests;

public:
	class Context;

public:
	/// Gets the singleton instance for the controller.
	static Controller& get();

	/// Executes the given natural language query.
	///
	/// \param[in] query The natural language query to execute.
	/// \param[in] context The task list context in which the query is run.
	/// \return The result of the query.
	/// \exception ContextRequiredException This is raised when a context is
	///                                     needed for the query to run.
	/// \see Result The result is polymorphic.
	Result query(const std::wstring& query, const Context& context) const;

	/// Gets all tasks with their IDs in the given list.
	///
	/// \remarks Order is preserved when returned.
	/// \param[in] taskIDs The task IDs to return.
	/// \return The tasks with the given ID.
	TaskList getTasks(const std::vector<Task::ID>& taskIDs) const;

	/// Gets all tasks.
	///
	/// \return The tasks currently in the data store.
	TaskList getTasks() const;

private:
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;
	Controller() = default;


private:
	/// Converts an NLP Task Priority to the equivalent Query Engine task
	/// priority.
	static Task::Priority nlpToQueryEnginePriority(NLP::TaskPriority priority);

private:
	/// The global singleton instance of the controller.
	static Controller instance;

	/// The map for converting NLP Task priorities to actual Query Engine
	/// task priorities.
	static const std::unordered_map<NLP::TaskPriority, Task::Priority>
		nlpToQueryEnginePriorityMap;
};

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_CONTROLLER_H_

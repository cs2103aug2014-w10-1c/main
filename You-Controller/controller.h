//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_CONTROLLER_H_
#define YOU_CONTROLLER_CONTROLLER_H_

#include <string>
#include "result.h"

namespace You {
namespace Controller {
namespace UnitTests { class ControllerContextTests; }

/// The entry-point for all communications with the rest of the You
/// framework.
///
/// This is a singleton class.
class Controller {
	friend class UnitTests::ControllerContextTests;

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
	Result getTasks(const std::vector<Task::ID>& taskIDs) const;

	/// Gets all tasks.
	///
	/// \return The tasks currently in the data store.
	Result getTasks() const;

private:
	Controller(const Controller&) = delete;
	Controller& operator=(const Controller&) = delete;
	Controller() = default;

private:
	/// The global singleton instance of the controller.
	static Controller instance;
};

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_CONTROLLER_H_

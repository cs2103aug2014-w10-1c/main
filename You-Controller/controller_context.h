#pragma once
#ifndef YOU_NLP_CONTROLLER_CONTEXT_H_
#define YOU_NLP_CONTROLLER_CONTEXT_H_

#include "controller.h"
#include "You-NLP/task_list.h"

namespace You {
namespace NLP {
namespace UnitTests { class ControllerContextTests; }

/// Represents a context of a query. This is usually a task list so that
/// relative indices can be used in a query.
class Controller::Context {
	friend class UnitTests::ControllerContextTests;

	/// The set of possible types that can form a context.
	typedef boost::variant<void*, TaskList> Types;

public:
	/// Task List conversion constructor.
	Context(const TaskList& taskList);  // NOLINT

	/// Checks if the given context is the default context.
	///
	/// \return true if the current context is the default context.
	bool isDefault() const;

	/// Checks if the given context is a task list context.
	///
	/// \return true if the current context is a task list context.
	/// \ref getTaskList
	bool isTaskList() const;

	/// Gets the task list if the current context is a task list
	/// context.
	///
	/// \return The task list if this is a task list context. Behaviour
	///         is undefined otherwise.
	const TaskList& getTaskList() const;

public:
	/// This is the global default context.
	static const Context DEFAULT;

private:
	/// Hidden default constructor. Use \ref Context::DEFAULT for the
	/// default context.
	///
	/// \see Context::DEFAULT
	Context();

private:
	/// The actual context.
	Types context;
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_CONTROLLER_CONTEXT_H_

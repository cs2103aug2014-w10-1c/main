//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_CONTROLLER_CONTEXT_H_
#define YOU_CONTROLLER_CONTROLLER_CONTEXT_H_

#include "controller.h"

namespace You {
namespace Controller {
namespace UnitTests { class ControllerContextTests; }

/// Represents a context of a query. This is usually a task list so that
/// relative indices can be used in a query.
class Controller::Context {
	friend class UnitTests::ControllerContextTests;

	/// The set of possible types that can form a context.
	typedef boost::variant<void*, TaskList> Types;

public:
	/// Task List conversion constructor.
	Context(const TaskList& taskList);  // NOLINT(runtime/explicit)

	/// Get the task at the given offset.
	///
	/// \param[in] offset The offset to retrieve.
	/// \exception ContextRequiredException When the given context is the
	///                                     default context.
	/// \exception std::out_of_range When the offset is larger than the number
	///                              of tasks present.
	const Task& at(size_t offset) const;

	/// Get the task at the given offset.
	///
	/// \param[in] offset The offset to retrieve.
	/// \exception ContextRequiredException When the given context is the
	///                                     default context.
	const Task& operator[](size_t offset) const;

public:
	/// This is the global default context.
	static const Context DEFAULT;

private:
	/// Hidden default constructor. Use \ref Context::DEFAULT for the
	/// default context.
	///
	/// \see Context::DEFAULT
	Context();

	/// Checks if the given context is the default context.
	///
	/// \return true if the current context is the default context.
	bool isDefault() const;

private:
	/// The actual context.
	Types context;
};

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_CONTROLLER_CONTEXT_H_

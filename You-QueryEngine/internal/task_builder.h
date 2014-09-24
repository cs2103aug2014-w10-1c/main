/// \file task_builder.h
/// Defines the task builder class
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_

#include "task_model.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// \brief Builder for tasks
///	New instance of task should be created using this builder.
///
/// Example usage:
///   \code{.cpp}
///	    Task newTask = Task::Builder::get().description("Feed");
///   \endcode
///
///	\note At least description must be provided when creating a task.
class Task::Builder {
	friend class Task;
public:
	/// Return a new builder instance.
	/// \return A builder object.
	static Builder get();

	/// Initialize task description .
	Builder& description(const Task::Description& description);
	/// Initialize task deadline.
	Builder& deadline(Task::Time deadline);
	/// Initialize task dependencies.
	Builder& dependencies(const Task::Dependencies& dependencies);
	/// Initialize task priority.
	Builder& priority(Task::Priority priority);

	~Builder() = default;
private:
	Builder();
	explicit Builder(const Task& instance) : instance(instance) {}

	/// Instance that will be returned by the builder
	Task instance;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_

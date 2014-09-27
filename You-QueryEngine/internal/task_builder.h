/// \file task_builder.h
/// Defines the task builder class
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_

#include "../task_model.h"

namespace You {
namespace QueryEngine {
namespace Internal {

///   Builder for tasks
///	  New instance of task should be created using this builder.
///
///   Example usage:
///   \code{.cpp}
///	    Task newTask = TaskBuilder::get().description("Feed");
///   \endcode
///
///	  \note At least description must be provided when creating a task.
class TaskBuilder {
public:
	/// Return a new builder instance.
	/// \return A builder object.
	static TaskBuilder get();

	/// Initialize task description .
	TaskBuilder& description(const Task::Description& description);
	/// Initialize task deadline.
	TaskBuilder& deadline(Task::Time deadline);
	/// Initialize task dependencies.
	TaskBuilder& dependencies(const Task::Dependencies& dependencies);
	/// Initialize task priority.
	TaskBuilder& priority(Task::Priority priority);

	/// Build task from its serialized form
	Task deserialize(const Task::Serialized& stask);
	/// Serialize a task to its serialized from
	Task::Serialized serialize(const Task& task);

	/// Convert the builder to task implicitly
	operator Task() const;

private:
	/// Disable default no-arg constructor.
	TaskBuilder();

	/// Copy constructor from instance.
	explicit TaskBuilder(const Task& instance) : instance(instance) {}

	/// Instance that will be returned by the builder
	Task instance;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_

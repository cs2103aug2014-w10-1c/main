/// \file task_builder.h
/// Defines the task builder class
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_CONTROLLER_TASK_BUILDER_H_
#define YOU_QUERYENGINE_INTERNAL_CONTROLLER_TASK_BUILDER_H_

#include "../model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class TaskBuilderTests;  }
namespace Internal {

/// Builder for tasks.
///	 New instance of task may be created using this builder.
///
///  Example usage:
///  \code{.cpp}
/// 	Task newTask = Controller::Builder::get().description("Feed");
///  \endcode
///
///	 \note At least description must be provided.
class TaskBuilder {
public:
	/// Return a new builder instance.
	/// \return A builder object.
	static TaskBuilder get();

	/// Start building from a current task object
	static TaskBuilder fromTask(const Task& task);

	/// Initialize task id.
	/// This also initialize parent's id to self.
	TaskBuilder& id(Task::ID id);
	/// Initialize task description .
	TaskBuilder& description(const Task::Description& description);
	/// Initialize task deadline.
	TaskBuilder& deadline(const Task::Time& deadline);
	/// Initialize task dependencies.
	TaskBuilder& dependencies(const Task::Dependencies& dependencies);
	/// Initialize task priority.
	TaskBuilder& priority(Task::Priority priority);
	/// Initialize task completed.
	TaskBuilder& completed(bool completed);
	/// Initialize task's parent.
	/// Parent id assumed exist.
	TaskBuilder& parent(const Task::ID id);
	/// Initialize task's subtasks.
	TaskBuilder& subtasks(const Task::Subtasks& subtasks);
	/// Initialize task's attachment
	TaskBuilder& attachment(const Task::Attachment& attachment);
	/// Convert the builder to task implicitly
	operator Task() const;

private:
	/// Disable default no-arg constructor.
	TaskBuilder();

	/// Copy constructor from instance.
	explicit TaskBuilder(const Task& instance)
	: instance(instance) {}

	/// Instance that will be returned by the builder
	Task instance;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_CONTROLLER_TASK_BUILDER_H_

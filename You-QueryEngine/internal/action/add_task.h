/// \file add_task.h
/// Defines action for adding tasks.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Define action for adding task
class AddTask : public Query {
public:
	/// Constructor
	explicit AddTask(Task::Description description, Task::Time deadline,
		Task::Priority priority, Task::Dependencies dependencies) :
		description(description), deadline(deadline), priority(priority),
		dependencies(dependencies) { }

	AddTask& operator=(const AddTask&) = delete;

private:
	/// Execute add task.
	Response execute() override;

	const Task::Description description;  ///< Description.
	const Task::Time deadline;  ///< Deadline.
	const Task::Priority priority;  ///< Priority.
	const Task::Dependencies dependencies;  ///< Dependencies.
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_

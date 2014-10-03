/// \file update_task.h
/// Defines action for updating tasks.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Define action for updating task
class UpdateTask : public Query {
public:
	/// Constructor
	explicit UpdateTask(Task::ID id, Task::Description description,
		Task::Time deadline, Task::Priority priority, Task::Dependencies dependencies)
		: id(id), description(description), deadline(deadline),
			priority(priority), dependencies(dependencies) { }

	/// Disable assignment operator
	UpdateTask& operator=(const UpdateTask&) = delete;

private:
	/// Execute add task.
	Response execute(State& const tasks) override;

	const Task::ID id;
	const Task::Description description;  ///< Description.
	const Task::Time deadline;  ///< Deadline.
	const Task::Priority priority;  ///< Priority.
	const Task::Dependencies dependencies;  ///< Dependencies.
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

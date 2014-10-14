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
namespace Action {

/// Action for updating task (e.g marking it as done)
class UpdateTask : public Query {
public:
	/// Construct EditTask query
	explicit UpdateTask(Task::ID id, Task::Description description,
		Task::Time deadline, Task::Priority priority, Task::Dependencies dependencies)
		: id(id), description(description), deadline(deadline),
			priority(priority), dependencies(dependencies) {}

   /// Construct mark task as completed/uncompleted
	explicit UpdateTask(Task::ID id, bool completed) : id(id),
		completed(completed) {}

	/// Disable assignment operator
	UpdateTask& operator=(const UpdateTask&) = delete;

	/// Destructor
	virtual ~UpdateTask() = default;

protected:
	static const std::wstring category;

private:
	Task buildUpdatedTask(const State& state) const;
	void modifyState(State& state, const Task& updated) const;
	void makeTransaction(const Task& updated) const;
	/// Execute add task.
	Response execute(State& tasks) override;

	const Task::ID id;
	const Task::Description description =
		Task::DEFAULT_DESCRIPTION;  ///< Description.
	const Task::Time deadline = Task::DEFAULT_DEADLINE;  ///< Deadline.
	const Task::Priority priority = Task::DEFAULT_PRIORITY;  ///< Priority.
	const Task::Dependencies dependencies =
		Task::DEFAULT_DEPENDENCIES;  ///< Dependencies.
	const bool completed = false;  ///< Completed
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

/// \file update_task.h
/// Defines action for updating tasks.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Update a task task.
/// (e.g changing its description or marking it as done)
class UpdateTask : public Query {
public:
	/// Constructor for UpdateTask query
	explicit UpdateTask(
		Task::ID id,
		You::Utils::Option<Task::Description> description,
		You::Utils::Option<Task::Time> deadline,
		You::Utils::Option<Task::Priority> priority,
		You::Utils::Option<Task::Dependencies> dependencies,
		You::Utils::Option<bool> completed,
		You::Utils::Option<Task::ID> parent,
		You::Utils::Option<Task::Subtasks> subtasks)
	: id(id),
	  description(description),
	  deadline(deadline),
	  priority(priority),
	  dependencies(dependencies),
	  completed(completed),
	  parent(parent),
	  subtasks(subtasks) {}

	/// Quick constructor for UpdateTask query
	explicit UpdateTask(const Task& task)
	: id(task.getID()),
	  description(task.getDescription()),
	  deadline(task.getDeadline()),
	  priority(task.getPriority()),
	  dependencies(task.getDependencies()),
	  completed(task.isCompleted()),
	  parent(task.getParent()),
	  subtasks(task.getSubtasks()) {}

	/// Disable assignment operator
	UpdateTask& operator=(const UpdateTask&) = delete;

	/// Destructor
	virtual ~UpdateTask() = default;

	Response execute(State& tasks) override;

protected:
	/// The reverse of updating is returning the original value.
	std::unique_ptr<Query> getReverse() override;

	/// The header of the log string
	static const std::wstring logCategory;

private:
	Task buildUpdatedTask(State& state) const;
	void updateDependencyGraph(State& state, const Task& updated) const;
	void updateSubtaskGraph(State& state, const Task& updated) const;
	void recMarkChildren(State& state, Task::ID id) const;
	void reparentTask(State& state, Task::ID id, Task::ID newParent) const;

	void makeTransaction(const Task& updated) const;

	const Task::ID id;
	const You::Utils::Option<Task::Description> description;  ///< Description.
	const You::Utils::Option<Task::Time> deadline;  ///< Deadline.
	const You::Utils::Option<Task::Priority> priority;  ///< Priority.
	const You::Utils::Option<Task::Dependencies> dependencies;  ///< Dependencies.
	const You::Utils::Option<bool> completed;  ///< Completed.
	const You::Utils::Option<Task::ID> parent;  ///< Parent.
	const You::Utils::Option<Task::Subtasks> subtasks;  ///< Parent.

	/// The previous state of the task.
	Task previous;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

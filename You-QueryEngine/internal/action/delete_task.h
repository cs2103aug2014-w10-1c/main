/// \file delete_task.h
/// Defines action for deleting tasks.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_DELETE_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_DELETE_TASK_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Action for deleting an existing task
/// Executing this action on non-existing task will throw
/// TaskNotFoundException.
class DeleteTask : public Query {
	friend class BatchDeleteSubTasks;
public:
	/// Construct the action using the task ID.
	explicit DeleteTask(Task::ID id)
	: id(id) {}

	/// Disable assignment operator.
	DeleteTask& operator=(const DeleteTask&) = delete;

	/// Destructor.
	virtual ~DeleteTask() = default;

protected:
	/// The reverse of deletion is adding.
	/// \return The query for adding the tasks (and their subtasks) back.
	std::unique_ptr<Query> getReverse() override;

	/// The header of the log string
	static const std::wstring logCategory;

private:
	/// Execute delete task.
	Response execute(State& tasks) override;
	void makeTransaction(const Task::ID id);
	void deleteTree(State& state, Task::ID id);
	void disconnectDependencies(State& state);
	/// ID of the task to be deleted.
	const Task::ID id;
	/// The task object that has been deleted.
	Task deletedTask;
	/// The mini database for children.
	std::unordered_map<Task::ID, Task> children;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_DELETE_TASK_H_

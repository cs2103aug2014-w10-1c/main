/// \file add_task.h
/// Defines action for adding tasks.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Action for adding a new task.
/// Three types of addition are supported.
/// 1. Adding single task.
/// 2. Adding single task alongside its dependencies.
/// 3. Adding single task alongside its subtasks.
class AddTask : public Query {
	friend class BatchAddSubTasks;
	friend class BatchDeleteSubTasks;
	friend class BatchAddDependencies;

public:
	/// Constructor for new Task, i.e the one without ID known.
	/// \see QueryEngine::AddTask
	/// \see QueryEngine::BatchAddDependencies
	/// \see QueryEngine::BatchAddSubtasks
	explicit AddTask(Task::Description description, Task::Time startTime,
		Task::Time deadline, Task::Priority priority,
		Task::Dependencies dependencies, Task::Subtasks subtasks)
	: insertedID(-1), description(description), startTime(startTime),
	  deadline(deadline), priority(priority), dependencies(dependencies),
	  subtasks(subtasks) {}

	/// Constructor for already known ID.
	/// \see QueryEngine::AddTask
	/// \see QueryEngine::BatchAddDependencies
	/// \see QueryEngine::BatchAddSubtasks
	explicit AddTask(Task::ID id, Task::Description description,
		Task::Time startTime, Task::Time deadline, Task::Priority priority,
		Task::Dependencies dependencies, Task::Subtasks subtasks)
	: insertedID(id), description(description), startTime(startTime),
	  deadline(deadline), priority(priority), dependencies(dependencies),
	  subtasks(subtasks) {}

	/// Disable assignment operator.
	AddTask& operator=(const AddTask&) = delete;

	/// Destructor.
	virtual ~AddTask() = default;

protected:
	/// The reverse of addition is deletion.
	/// \returns The delete query as a reverse of this action.
	std::unique_ptr<Query> getReverse() override;

	/// The header of the log string.
	/// \returns The log category string.
	static const std::wstring logCategory;

private:
	Task buildTask(const Task::ID id);

	void addTaskToGraphs(const Task& task, State& state) const;
	void makeTransaction(const Task& newTask) const;
	void ensureDependencyIsValid() const;
	void ensureSubtasksIsValid() const;
	void updateParentPointer(State& state) const;

	/// Execute add task.
	Response execute(State& tasks) override;

	Task::ID insertedID;  ///< Inserted ID
	const Task::Description description;  ///< Description.
	const Task::Time startTime;  ///< Deadline.
	const Task::Time deadline;  ///< Deadline.
	const Task::Priority priority;  ///< Priority.
	const Task::Dependencies dependencies;  ///< Dependencies.
	const Task::Subtasks subtasks;  ///< Subtasks.
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_

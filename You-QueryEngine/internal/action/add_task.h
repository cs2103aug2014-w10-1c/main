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
namespace Action {

/// Action for adding a new task
class AddTask : public Query {
	friend class BatchAddSubTasks;
	friend class BatchDeleteSubTasks;
	friend class BatchAddDependencies;

public:
	/// Constructor that use datastore to inquire new id
	explicit AddTask(Task::Description description, Task::Time deadline,
		Task::Priority priority, Task::Dependencies dependencies,
		Task::Subtasks subtasks)
	: insertedID(-1), description(description),
	  deadline(deadline), priority(priority), dependencies(dependencies),
	  subtasks(subtasks) {}

	/// Constructor for already known id
	explicit AddTask(Task::ID id, Task::Description description,
		Task::Time deadline, Task::Priority priority,
		Task::Dependencies dependencies, Task::Subtasks subtasks)
	: insertedID(id), description(description), deadline(deadline),
	  priority(priority), dependencies(dependencies), subtasks(subtasks) {}

	/// Disable assignment operator
	AddTask& operator=(const AddTask&) = delete;

	/// Destructor
	virtual ~AddTask() = default;

protected:
	/// The reverse of addition is deletion.
	std::unique_ptr<Query> getReverse() override;

	/// The header of the log string
	static const std::wstring logCategory;

private:
	Task buildTask(const Task::ID id);

	void addTaskToGraphs(const Task& task, State& state) const;
	void makeTransaction(const Task& newTask) const;
	void ensureDependencyIsValid() const;
	void ensureSubtasksIsValid() const;
	void updateParentPointer() const;

	/// Execute add task.
	Response execute(State& tasks) override;

	Task::ID insertedID;  ///< Inserted ID
	const Task::Description description;  ///< Description.
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

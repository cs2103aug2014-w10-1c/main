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
	explicit UpdateTask(
		Task::ID id,
		You::Utils::Option<Task::Description> description,
		You::Utils::Option<Task::Time> deadline,
		You::Utils::Option<Task::Priority> priority,
		You::Utils::Option<Task::Dependencies> dependencies,
		You::Utils::Option<bool> completed) :
		id(id),
		description(description),
		deadline(deadline),
		priority(priority),
		dependencies(dependencies),
		completed(completed) {}

	/// Disable assignment operator
	UpdateTask& operator=(const UpdateTask&) = delete;

	/// Destructor
	virtual ~UpdateTask() = default;

protected:
	/// The reverse of updating is returning the original value.
	std::unique_ptr<Query> getReverse() override;

	static const std::wstring logCategory;

private:
	Task buildUpdatedTask(const State& state) const;
	void modifyState(State& state, const Task& updated) const;
	void makeTransaction(const Task& updated) const;
	/// Execute add task.
	Response execute(State& tasks) override;

	const Task::ID id;
	const You::Utils::Option<Task::Description> description;  ///< Description.
	const You::Utils::Option<Task::Time> deadline;  ///< Deadline.
	const You::Utils::Option<Task::Priority> priority;  ///< Priority.
	const You::Utils::Option<Task::Dependencies> dependencies;  ///< Dependencies.
	const You::Utils::Option<bool> completed = false;  ///< Completed.
	/// The previous state of the task.
	Task previous;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

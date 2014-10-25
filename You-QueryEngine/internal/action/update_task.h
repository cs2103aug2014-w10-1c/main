/// \file update_task.h
/// Defines action for updating tasks.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

#include <boost/optional.hpp>
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
		boost::optional<Task::Description> description,
		boost::optional<Task::Time> deadline,
		boost::optional<Task::Priority> priority,
		boost::optional<Task::Dependencies> dependencies,
		boost::optional<bool> completed) :
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
	const boost::optional<Task::Description> description;  ///< Description.
	const boost::optional<Task::Time> deadline;  ///< Deadline.
	const boost::optional<Task::Priority> priority;  ///< Priority.
	const boost::optional<Task::Dependencies> dependencies;  ///< Dependencies.
	const boost::optional<bool> completed = false;  ///< Completed.
	/// The previous state of the task.
	Task previous;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_UPDATE_TASK_H_

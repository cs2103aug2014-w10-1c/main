/// \file batch_add_subtasks.h
/// Defines action for adding a batch of subtasks.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_ADD_SUBTASKS_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_ADD_SUBTASKS_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Action for adding a new task
class BatchAddSubTasks : public Query {
public:
	/// Constructor that use datastore to inquire new id
	explicit BatchAddSubTasks(
		const Task::Description& description,
		const Task::Time& deadline,
		const Task::Priority& priority,
		const Task::Dependencies& dependencies,
		std::vector<std::unique_ptr<Query>>&& subtasks)
	: insertedID(-1), description(description), deadline(deadline),
	  priority(priority), dependencies(dependencies),
	  subtasks(std::move(subtasks)) {}

	/// Constructor for already known id
	explicit BatchAddSubTasks(
		Task::ID id,
		const Task::Description& description,
		const Task::Time& deadline,
		const Task::Priority& priority,
		const Task::Dependencies& dependencies,
		std::vector<std::unique_ptr<Query>>&& subtasks)
	: insertedID(id), description(description), deadline(deadline),
		priority(priority), dependencies(dependencies),
		subtasks(std::move(subtasks)) {}

	/// Disable assignment operator
	BatchAddSubTasks& operator=(const BatchAddSubTasks&) = delete;

	/// Destructor
	virtual ~BatchAddSubTasks() = default;

protected:
	/// The reverse of batch addition is batch deletion.
	std::unique_ptr<Query> getReverse() override;

	/// The header of the log string
	static const std::wstring logCategory;

private:
	Task::Subtasks executeSubtasksAddQuery(State& state) const;
	Task executeParentAddQuery(State& state,
		const Task::Subtasks& subtasks) const;

	/// Execute add task.
	Response execute(State& tasks) override;

	Task::ID insertedID;   ///< Inserted ID of the parent task.
	const Task::Description description;  ///< Description.
	const Task::Time deadline;  ///< Deadline.
	const Task::Priority priority;  ///< Priority.
	const Task::Dependencies dependencies;  ///< Dependencies.
	std::vector<std::unique_ptr<Query>> subtasks;	 ///< Subtasks
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_ADD_SUBTASKS_H_

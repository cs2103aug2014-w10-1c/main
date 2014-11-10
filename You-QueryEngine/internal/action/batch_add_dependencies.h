/// \file batch_add_subtasks.h
/// Defines action for adding a batch of dependencies.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_ADD_DEPENDENCIES_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_ADD_DEPENDENCIES_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Action for adding a new task alongside dependencies.
class BatchAddDependencies : public Query {
public:
	/// Constructor that use datastore to inquire new id
	explicit BatchAddDependencies(
		const Task::Description& description,
		const Task::Time& startTime,
		const Task::Time& deadline,
		const Task::Priority& priority,
		std::vector<std::unique_ptr<Query>>&& dependencies,
		const Task::Subtasks& subtasks)
	: startTime(startTime), description(description), deadline(deadline),
	  priority(priority), dependencies(std::move(dependencies)),
	  subtasks(subtasks) {}

	/// Disable assignment operator
	BatchAddDependencies& operator=(const BatchAddDependencies&) = delete;

	/// Destructor
	virtual ~BatchAddDependencies() = default;

protected:
	/// The header of the log string
	static const std::wstring logCategory;

	std::unique_ptr<Query> getReverse() override;

private:
	Task executeParentAddQuery(State& state);
	Task::ID executeDependenciesAddQuery(State& state);
	/// Execute add task.
	Response execute(State& tasks) override;

	std::vector<Task::ID> insertedIDs;  ///< List of inserted IDs
	const Task::Description description;  ///< Description.
	const Task::Time startTime;  ///< Deadline.
	const Task::Time deadline;  ///< Deadline.
	const Task::Priority priority;  ///< Priority.
	std::vector<std::unique_ptr<Query>> dependencies;  ///< Dependencies
	const Task::Subtasks subtasks;  ///< Subtasks.
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_ADD_DEPENDENCIES_H_

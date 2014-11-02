/// \file batch_delete_subtasks.h
/// Defines action for adding a batch of subtasks.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_DELETE_SUBTASKS_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_DELETE_SUBTASKS_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Action for adding a new task
class BatchDeleteSubTasks : public Query {
public:
	/// Constructor that use datastore to inquire new id
	explicit BatchDeleteSubTasks(const Task::ID id)
	: id(id), deletedParent() {}

	/// Disable assignment operator
	BatchDeleteSubTasks& operator=(const BatchDeleteSubTasks&) = delete;

	/// Destructor
	virtual ~BatchDeleteSubTasks() = default;

protected:
	/// The reverse of batch deletion is batch addition.
	std::unique_ptr<Query> getReverse() override;

	/// The header of the log string
	static const std::wstring logCategory;

private:
	/// Delete tree
	void deleteTree(State& state, Task::ID id);
	void constructUndoTree(State& state, Task::ID id);

	/// Execute add task.
	Response execute(State& tasks) override;
	Task::ID id;   ///< Inserted ID of the parent task.
	Task deletedParent;   ///< Deleted parent
	std::unordered_map<Task::ID, Task> childrens;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_BATCH_DELETE_SUBTASKS_H_

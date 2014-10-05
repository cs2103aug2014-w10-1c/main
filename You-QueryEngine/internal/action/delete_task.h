/// \file delete_task.h
/// Defines action for deleting tasks.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_DELETE_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_DELETE_TASK_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Define action for deleting task
class DeleteTask : public Query {
public:
	/// Constructor
	explicit DeleteTask(Task::ID id) : id(id) { }

	/// Disable assignment operator
	DeleteTask& operator=(const DeleteTask&) = delete;

	/// Destructor
	virtual ~DeleteTask() = default;
private:
	/// Execute delete task.
	Response execute(State& tasks) override;

	const Task::ID id;  ///< ID of the task to delete
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_DELETE_TASK_H_

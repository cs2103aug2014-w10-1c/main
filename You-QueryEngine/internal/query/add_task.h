/// \file add_task.h
/// Defines the action interface.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_

#include <vector>

#include "../../query.h"
#include "../../task_model.h"
#include "../action.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Add task to the task list.
class AddTask : public IAction<Query::AddTask, Task> {
public:
	/// Execute and mutate data.
	/// \returns A response object of type TResponse
	Task execute() override;

	AddTask(Task::Description description, Task::Time deadline,
		Task::Priority priority, Task::Dependencies dependencies);
private:
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_ADD_TASK_H_

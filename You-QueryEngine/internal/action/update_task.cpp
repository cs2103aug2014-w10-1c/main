/// \author A0112054Y
#include "stdafx.h"

#include "../task_builder.h"
#include "../task_serializer.h"
#include "../state.h"
#include "update_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

Response UpdateTask::execute(State& tasks) {
	auto current = tasks.get().graph().getTask(this->id);
	auto builder = TaskBuilder::fromTask(current);
	if (this->description == Task::DEFAULT_DESCRIPTION) {
		builder.description(current.getDescription());
	} else {
		builder.description(this->description);
	}
	if (this->deadline != Task::DEFAULT_DEADLINE) {
		builder.deadline(this->deadline);
	}
	if (this->priority != Task::DEFAULT_PRIORITY) {
		builder.priority(this->priority);
	}
	if (this->dependencies != Task::DEFAULT_DEPENDENCIES) {
		builder.dependencies(this->dependencies);
	}
	Task newTask = builder;
	newTask.setCompleted(this->completed);
	tasks.get().graph().updateTask(newTask);
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

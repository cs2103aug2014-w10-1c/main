/// \author A0112054Y
#include "stdafx.h"

#include "../task_builder.h"
#include "../task_serializer.h"
#include "../task_serializer.h"
#include "../state.h"
#include "add_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

Response AddTask::execute(State& tasks) {
	Task newTask = TaskBuilder::get()
		.id(tasks.inquireNewID())
		.description(this->description)
		.deadline(this->deadline)
		.dependencies(this->dependencies)
		.priority(this->priority);
	tasks.graph().addTask(newTask);
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

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
		.description(this->description)
		.deadline(this->deadline)
		.dependencies(this->dependencies)
		.priority(this->priority);
	tasks.graph().addTask(newTask);

#if 0
	auto serialized = TaskSerializer::serialize(newTask);
	DataStorage::Transaction t(DataStorage::begin());
	DataStorage::post(serialized);
	t.commit();
#endif

	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

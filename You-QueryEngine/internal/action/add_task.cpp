/// \author A0112054Y
#include "stdafx.h"

#include "add_task.h"
#include "../task_builder.h"
#include "../task_serializer.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Response AddTask::execute(State& const tasks) {
	Task newTask = TaskBuilder::get()
		.description(this->description)
		.deadline(this->deadline)
		.dependencies(this->dependencies)
		.priority(this->priority);
	auto serialized = TaskSerializer::serialize(newTask);
#if 0
	DataStorage::Transaction t(DataStorage::begin());
	DataStorage::post(serialized);
	t.commit();
#endif
	return newTask;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

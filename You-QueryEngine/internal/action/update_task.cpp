/// \author A0112054Y
#include "stdafx.h"

#include "../task_builder.h"
#include "../task_serializer.h"
#include "update_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Response UpdateTask::execute() {
	Task newTask = TaskBuilder::get()
		.id(this->id)
		.description(this->description)
		.deadline(this->deadline)
		.dependencies(this->dependencies)
		.priority(this->priority);
	auto serialized = TaskSerializer::serialize(newTask);
#if 0
	queryEngine.getTaskGraph().update(this->id, newTask);
#endif

#if 0
	Transaction t(dataStorage.begin());
	dataStorage.put(this->id, serialized);
	t.commit();
#endif
	return this->id;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

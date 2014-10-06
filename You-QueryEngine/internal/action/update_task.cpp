/// \author A0112054Y
#include "stdafx.h"

#include "../task_builder.h"
#include "../task_serializer.h"
#include "../exception.h"
#include "update_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

Response UpdateTask::execute(State& tasks) {
	auto builder = TaskBuilder::get().id(this->id);
	if (this->description == Task::DEFAULT_DESCRIPTION) {
		/// This should get the description of the
		/// current id of the task
		builder.description(L"Not implemented");
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
#if 0
	queryEngine.getTaskGraph().update(this->id, newTask);
#endif

#if 0
	Transaction t(dataStorage.begin());
	dataStorage.put(this->id, serialized);
	t.commit();
#endif
	return (Task) builder;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

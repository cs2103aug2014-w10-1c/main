#include "stdafx.h"
#include "exception.h"
#include "task_builder.h"

namespace You {
namespace QueryEngine {
namespace Internal {

TaskBuilder TaskBuilder::get() {
	TaskBuilder builder(Task::nextNewTask());
	return builder;
}

TaskBuilder& TaskBuilder::description(const Task::Description &description) {
	instance.setDescription(description);
	return *this;
}

TaskBuilder& TaskBuilder::deadline(Task::Time deadline) {
	instance.setDeadline(deadline);
	return *this;
}

TaskBuilder& TaskBuilder::dependencies(const Task::Dependencies &dependencies) {
	instance.setDependencies(dependencies);
	return *this;
}

TaskBuilder& TaskBuilder::priority(Task::Priority priority) {
	instance.setPriority(priority);
	return *this;
}

TaskBuilder::operator Task() const {
	bool isEmptyDescription =
		instance.description == Task::DEFAULT_DESCRIPTION;
	if (isEmptyDescription) {
		throw Internal::EmptyTaskDescriptionException();
	}
	return instance;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

/// \author A0112054Y
#include "stdafx.h"

#include "../model.h"
#include "../exception.h"
#include "task_builder.h"

namespace You {
namespace QueryEngine {
namespace Internal {

TaskBuilder TaskBuilder::get() {
	Task defaultTask = Task(
		Task::DEFAULT_ID,
		Task::DEFAULT_DESCRIPTION,
		Task::DEFAULT_DEADLINE,
		Task::DEFAULT_DEPENDENCIES,
		Task::DEFAULT_PRIORITY,
		Task::DEFAULT_ID,
		Task::DEFAULT_SUBTASKS);
	TaskBuilder builder(defaultTask);
	return builder;
}

TaskBuilder TaskBuilder::fromTask(const Task& task) {
	return TaskBuilder(task);
}

TaskBuilder& TaskBuilder::id(Task::ID id) {
	instance.id = id;
	instance.parent = id;
	return *this;
}

TaskBuilder& TaskBuilder::description(const Task::Description &description) {
	instance.setDescription(description);
	return *this;
}

TaskBuilder& TaskBuilder::deadline(const Task::Time& deadline) {
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

TaskBuilder& TaskBuilder::parent(const Task::ID parent) {
	instance.setParent(parent);
	return *this;
}

TaskBuilder& TaskBuilder::subtasks(const Task::Subtasks &subtasks) {
	instance.setSubtasks(subtasks);
	return *this;
}

TaskBuilder::operator Task() const {
	bool isEmptyDescription =
		instance.description == Task::DEFAULT_DESCRIPTION;
	if (isEmptyDescription) {
		throw Exception::EmptyTaskDescriptionException();
	}
	return instance;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

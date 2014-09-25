#include "stdafx.h"
#include "exception.h"
#include "task_builder.h"

namespace You {
namespace QueryEngine {
namespace Internal {

using Builder = Task::Builder;

Builder Task::Builder::get() {
	Builder builder(nextNewTask());
	return builder;
}

Builder& Task::Builder::description(const Task::Description &description) {
	instance.description = description;
	return *this;
}

Builder& Task::Builder::deadline(Task::Time deadline) {
	instance.deadline = deadline;
	return *this;
}

Builder& Task::Builder::dependencies(const Task::Dependencies &dependencies) {
	instance.dependencies = dependencies;
	return *this;
}

Builder& Task::Builder::priority(Task::Priority priority) {
	instance.priority = priority;
	return *this;
}

Task::Task(const Builder& builder) {
	bool taskAlreadyExist = false;  // TODO(evansb) Change this when DS ready.
	bool isEmptyDescription = builder.instance.description == DEFAULT_DESCRIPTION;

	if (isEmptyDescription) {
		throw EmptyTaskDescriptionException();
	} else {
		this->id = builder.instance.id;
		this->description = builder.instance.description;
	}

	this->dependencies = builder.instance.dependencies;
	this->deadline = builder.instance.deadline;
	this->priority = builder.instance.priority;
}

Task& Task::operator=(const Builder& builder) {
	return *this = Task(builder);
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

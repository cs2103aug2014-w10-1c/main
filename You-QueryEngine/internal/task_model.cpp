#include "stdafx.h"
#include "task_model.h"

namespace You {
namespace QueryEngine {

using Builder = Task::Builder;

void Task::setDescription(const Task::Description &description) {
	this->description = description;
}

void Task::setDeadline(Task::Time time) {
	this->deadline = time;
}

void Task::setDependencies(const Task::Dependencies &dependencies) {
	this->dependencies = dependencies;
}

void Task::setPriority(Task::Priority priority) {
	this->priority = priority;
}

Task Task::nextNewTask() {
	return Task(
		DUMMY_ID,
		DEFAULT_DESCRIPTION,
		DEFAULT_DEADLINE,
		DEFAULT_DEPENDENCIES,
		DEFAULT_PRIORITY);
}

Task Builder::instance = Task::nextNewTask();

Builder&& Builder::get() {
	static Builder builder;
	return std::move(builder);
}

Builder&& Builder::id(Task::ID id) {
	instance.id = id;
	return get();
}

Builder&& Builder::description(const Task::Description &description) {
	instance.description = description;
	return get();
}

Builder&& Builder::deadline(Task::Time deadline) {
	instance.deadline = deadline;
	return get();
}

Builder&& Builder::dependencies(const Task::Dependencies &dependencies) {
	instance.dependencies = dependencies;
	return get();
}

Builder&& Builder::priority(Task::Priority priority) {
	instance.priority = priority;
	return get();
}

Task::Task(const Builder &&builder) {
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

	builder.instance = Task::nextNewTask();
}

Task&& Task::operator=(const Builder &&builder) {
	return Task(std::move(builder));
}

const Task::Description Task::DEFAULT_DESCRIPTION = L"";
const Task::Time Task::DEFAULT_DEADLINE = INT64_MAX;
const Task::Dependencies Task::DEFAULT_DEPENDENCIES;
const Task::Priority Task::DEFAULT_PRIORITY = Task::Priority::NORMAL;

const Task::ID Task::DUMMY_ID = 42L;

}  // namespace QueryEngine
}  // namespace You

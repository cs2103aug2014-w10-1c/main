/// \author A0112054Y
#include "stdafx.h"
#include "internal/model.h"
#include "task_model.h"

namespace {
	using boost::posix_time::ptime;
	using boost::posix_time::hours;
	using boost::gregorian::date;
	using boost::gregorian::max_date_time;
	using boost::gregorian::day_clock;
}  // namespace

namespace You {
namespace QueryEngine {

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

void Task::setCompleted(bool completed) {
	this->completed = completed;
}

void Task::setSubtasks(const Subtasks& subtasks) {
	this->subtasks = subtasks;
}

void Task::setParent(const Task::ID parent) {
	this->parent = parent;
}

bool Task::isDependOn(const Task::ID id) const {
	return dependencies.find(id) != dependencies.end();
}

std::vector<Task> Task::getSubtasksObject() {
	std::vector<Task> result;
	for (const auto& id : subtasks) {
		result.push_back(Internal::State::get().graph().getTask(id));
	}
	return result;
}

std::vector<Task> Task::getDependenciesObject() {
	std::vector<Task> result;
	for (const auto& id : dependencies) {
		result.push_back(Internal::State::get().graph().getTask(id));
	}
	return result;
}

const Task::ID Task::DEFAULT_ID = 0L;
const Task::Description Task::DEFAULT_DESCRIPTION = L"";
const Task::Time Task::NEVER = ptime(date(max_date_time), hours(0));
const Task::Time Task::DEFAULT_DEADLINE = Task::NEVER;
const Task::Dependencies Task::DEFAULT_DEPENDENCIES;
const Task::Priority Task::DEFAULT_PRIORITY = Task::Priority::NORMAL;
const Task::Subtasks Task::DEFAULT_SUBTASKS;

bool Task::isStrictEqual(const Task& task) const {
	bool idIsEqual = id == task.id;
	bool descriptionIsEqual = description == task.description;
	bool priorityIsEqual = priority == task.priority;
	bool dependenciesIsEqual = dependencies == task.dependencies;
	bool deadlineIsEqual = deadline == task.deadline;
	bool completedIsEqual = completed == task.completed;
	bool subtasksIsEqual = subtasks == task.subtasks;
	return idIsEqual && descriptionIsEqual && priorityIsEqual
		&& dependenciesIsEqual && deadlineIsEqual && subtasksIsEqual
		&& completedIsEqual;
}

}  // namespace QueryEngine
}  // namespace You

/// \author A0112054Y
#include "stdafx.h"

#include "internal/controller.h"
#include "task_model.h"

using boost::posix_time::ptime;
using boost::posix_time::hours;
using boost::gregorian::date;
using boost::gregorian::max_date_time;

namespace {
	const std::wstring TASK_FORMAT = L"[%1%][%2%][%3%][%4%][%5%]";
}  // namespace

namespace You {
namespace QueryEngine {

Task::Task() {
}

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

const Task::ID Task::DEFAULT_ID = 0L;
const Task::Description Task::DEFAULT_DESCRIPTION = L"";
const Task::Time Task::NEVER = ptime(date(max_date_time), hours(0));
const Task::Time Task::DEFAULT_DEADLINE = Task::NEVER;
const Task::Dependencies Task::DEFAULT_DEPENDENCIES;
const Task::Priority Task::DEFAULT_PRIORITY = Task::Priority::NORMAL;

bool Task::isStrictEqual(const Task& task) const {
	bool idIsEqual = id == task.id;
	bool descriptionIsEqual = description == task.description;
	bool priorityIsEqual = priority == task.priority;
	auto d1 = std::unordered_set<Task::ID>(dependencies.begin(),
		dependencies.end());
	auto d2 = std::unordered_set<Task::ID>(task.dependencies.begin(),
		task.dependencies.end());
	bool dependenciesIsEqual = d1 == d2;
	bool deadlineIsEqual = deadline == task.deadline;
	return idIsEqual && descriptionIsEqual && priorityIsEqual
		&& dependenciesIsEqual && deadlineIsEqual;
}

std::wstring ToString(const Task& task) {
	using Serializer = Internal::TaskSerializer;
	auto serialized = Serializer::serialize(task);
	return (boost::wformat(TASK_FORMAT)
		% serialized.at(Serializer::KEY_ID)
		% serialized.at(Serializer::KEY_DESCRIPTION)
		% serialized.at(Serializer::KEY_PRIORITY)
		% serialized.at(Serializer::KEY_DEADLINE)
		% serialized.at(Serializer::KEY_DEPENDENCIES)).str();
}

}  // namespace QueryEngine
}  // namespace You

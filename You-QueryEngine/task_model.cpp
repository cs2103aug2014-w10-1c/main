/// \author A0112054Y
#include "stdafx.h"

#include "internal/task_serializer.h"
#include "task_model.h"

using boost::posix_time::ptime;
using boost::posix_time::hours;
using boost::gregorian::date;
using boost::gregorian::max_date_time;

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
	bool dependenciesIsEqual = dependencies == task.dependencies;
	bool deadlineIsEqual = deadline == task.deadline;
	return idIsEqual && descriptionIsEqual && priorityIsEqual
		&& dependenciesIsEqual && deadlineIsEqual;
}

const std::wstring Task::FORMAT = L"[%1%][%2%][%3%][%4%][%5%]";

std::wstring Task::ToString(const Task& task) {
	using Serializer = Internal::TaskSerializer;
	auto serialized = Serializer::serialize(task);
	return (boost::wformat(FORMAT)
		% serialized.at(Serializer::KEY_ID)
		% serialized.at(Serializer::KEY_DESCRIPTION)
		% serialized.at(Serializer::KEY_PRIORITY)
		% serialized.at(Serializer::KEY_DEADLINE)
		% serialized.at(Serializer::KEY_DEPENDENCIES)).str();
}

}  // namespace QueryEngine
}  // namespace You

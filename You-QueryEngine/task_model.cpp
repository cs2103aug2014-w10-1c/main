/// \author A0112054Y
#include "stdafx.h"
#include "internal/controller.h"
#include "filter.h"
#include "task_model.h"

namespace {
	using boost::posix_time::ptime;
	using boost::posix_time::hours;
	using boost::gregorian::date;
	using boost::gregorian::max_date_time;
	using boost::gregorian::day_clock;

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

bool Task::isDependOn(const Task::ID id) const {
	return dependencies.find(id) != dependencies.end();
}

std::wstring Task::getDeadlineString() const {
	std::wstring result;
	std::int32_t dayDue =
		static_cast<std::int32_t>(deadline.date().julian_day());
	std::int32_t dayNow =
		static_cast<std::int32_t>(day_clock::local_day().julian_day());
	std::int32_t daysLeft = dayDue - dayNow;

	if (deadline == NEVER) {
		result = DEADLINE_NEVER;
	} else if ((Filter::overdue())(*this)) {
		result = boost::wformat(DEADLINE_OVERDUE).str();
	} else if (daysLeft < 7) {
		result = (boost::wformat(DEADLINE_WITHIN_DAYS) % daysLeft).str();
	} else if (daysLeft < 30) {
		std::int32_t weekDifference =
			static_cast<std::int32_t>(floor(daysLeft / 7.0));
		result = (boost::wformat(DEADLINE_OVERDUE) % weekDifference).str();
	} else {
		std::int32_t monthDifference =
			static_cast<std::int32_t>(floor(daysLeft / 30.0));
		result = (boost::wformat(DEADLINE_OVERDUE) % monthDifference).str();
	}
	return result;
}

const Task::ID Task::DEFAULT_ID = 0L;
const Task::Description Task::DEFAULT_DESCRIPTION = L"";
const Task::Time Task::NEVER = ptime(date(max_date_time), hours(0));
const Task::Time Task::DEFAULT_DEADLINE = Task::NEVER;
const Task::Dependencies Task::DEFAULT_DEPENDENCIES;
const Task::Priority Task::DEFAULT_PRIORITY = Task::Priority::NORMAL;

const std::wstring Task::DEADLINE_OVERDUE = L"overdue by %1%";
const std::wstring Task::DEADLINE_TODAY = L"today";
const std::wstring Task::DEADLINE_WITHIN_DAYS = L"in %1% days";
const std::wstring Task::DEADLINE_WITHIN_MONTHS = L"in %1% months";
const std::wstring Task::DEADLINE_NEVER = L"never";

bool Task::isStrictEqual(const Task& task) const {
	bool idIsEqual = id == task.id;
	bool descriptionIsEqual = description == task.description;
	bool priorityIsEqual = priority == task.priority;
	bool dependenciesIsEqual = dependencies == task.dependencies;
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

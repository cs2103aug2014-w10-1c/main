#include "stdafx.h"

#include "task_serializer.h"

namespace You {
namespace QueryEngine {
namespace Internal {

using boost::posix_time::ptime;
using boost::gregorian::date;
using boost::gregorian::greg_year_month_day;
using boost::posix_time::time_duration;

using TS = TaskSerializer;

const TS::Key TS::KEY_DESCRIPTION = L"description";
const TS::Key TS::KEY_DEADLINE = L"deadline";
const TS::Key TS::KEY_PRIORITY = L"priority";
const TS::Key TS::KEY_DEPENDENCIES = L"dependencies";

TS::STask TS::serialize(const Task& task) {
	Value value_description = serializeDescription(task.getDescription());
	Value value_deadline = serializeDeadline(task.getDeadline());
	Value value_priority = serializePriority(task.getPriority());
	Value value_dependencies = serializeDependencies(task.getDependencies());
	return {
		{ KEY_DESCRIPTION, value_description },
		{ KEY_DEADLINE, value_deadline },
		{ KEY_PRIORITY, value_priority },
		{ KEY_DEPENDENCIES, value_dependencies },
	};
}

const TS::Value TS::VALUE_PRIORITY_NORMAL = L"normal";
const TS::Value TS::VALUE_PRIORITY_IMPORTANT = L"important";
const TS::Value TS::VALUE_DELIMITER = L";";

TS::Value TS::serializeID(Task::ID id) {
	return boost::lexical_cast<std::wstring>(id);
}

TS::Value TS::serializeDescription(Task::Description description) {
	return description;
}

TS::Value TS::serializeDeadline(Task::Time deadline) {
	std::wstringstream wss;
	auto date = deadline.date();
	auto time = deadline.time_of_day();
	std::vector<int> fields = {
		date.year(), date.month(), date.day(),
		time.hours(), time.minutes(), time.seconds()
	};
	std::for_each(fields.begin(), fields.end(),
		[&wss](int m) { wss << m << TS::VALUE_DELIMITER; });
	return wss.str();
}

TS::Value TS::serializePriority(Task::Priority priority) {
	if (priority == Task::Priority::NORMAL) {
		return VALUE_PRIORITY_NORMAL;
	} else {
		return VALUE_PRIORITY_IMPORTANT;
	}
}

TS::Value TS::serializeDependencies(Task::Dependencies dependencies) {
	std::wstringstream ws;
	std::for_each(dependencies.cbegin(), dependencies.cend(),
		[&ws] (const Task::ID id) {
			ws << TS::serializeID(id);
			ws << TS::VALUE_DELIMITER;
		}
	);
	return ws.str();
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#include "stdafx.h"

#include "task_serializer.h"

namespace You {
namespace QueryEngine {
namespace Internal {

using boost::posix_time::to_simple_string;
using TS = TaskSerializer;

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

const TS::Key KEY_DESCRIPTION = L"description";
const TS::Key KEY_DEADLINE = L"deadline";
const TS::Key KEY_PRIORITY = L"priority";
const TS::Key KEY_DEPENDENCIES = L"dependencies";

const TS::Value VALUE_PRIORITY_NORMAL = L"normal";
const TS::Value VALUE_PRIORITY_IMPORTANT = L"important";
const TS::Value VALUE_DEPENDENCIES_DELIMITER = L";";

TS::Value TS::serializeID(Task::ID id) {
	return boost::lexical_cast<std::wstring>(id);
}

TS::Value TS::serializeDescription(Task::Description description) {
	return description;
}

TS::Value TS::serializeDeadline(Task::Time deadline) {
	TS::Value r;
	std::string ds = to_simple_string(deadline);
	return r.assign(ds.cbegin(), ds.cend());
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
			ws << TS::VALUE_DEPENDENCIES_DELIMITER;
		}
	);
	return ws.str();
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#include "stdafx.h"

#include <string>
#include <unordered_map>
#include <boost/tokenizer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "task_builder.h"
#include "task_serializer.h"

namespace You {
namespace QueryEngine {
namespace Internal {

namespace {
	using boost::posix_time::ptime;
	using boost::gregorian::date;
	using boost::gregorian::greg_year_month_day;
	using boost::posix_time::time_duration;

	using TS = TaskSerializer;
}

const TS::Key TS::KEY_ID = L"id";
const TS::Key TS::KEY_DESCRIPTION = L"description";
const TS::Key TS::KEY_DEADLINE = L"deadline";
const TS::Key TS::KEY_PRIORITY = L"priority";
const TS::Key TS::KEY_DEPENDENCIES = L"dependencies";
const TS::Key TS::KEY_PARENT = L"parent";
const TS::Key TS::KEY_SUBTASKS = L"subtasks";

const TS::Value TS::VALUE_PRIORITY_NORMAL = L"normal";
const TS::Value TS::VALUE_PRIORITY_HIGH = L"high";
const TS::Value TS::VALUE_DELIMITER = L";";

TS::STask TS::serialize(const Task& task) {
	Value value_id = serializeID(task.getID());
	Value value_description = serializeDescription(task.getDescription());
	Value value_deadline = serializeDeadline(task.getDeadline());
	Value value_priority = serializePriority(task.getPriority());
	Value value_dependencies = serializeDependencies(task.getDependencies());
	Value value_subtasks = serializeSubtasks(task.getSubtasks());
	Value value_parent = serializeParent(task.getParent());
	return {
		{ KEY_ID, value_id },
		{ KEY_DESCRIPTION, value_description },
		{ KEY_DEADLINE, value_deadline },
		{ KEY_PRIORITY, value_priority },
		{ KEY_DEPENDENCIES, value_dependencies },
		{ KEY_PARENT, value_parent },
		{ KEY_SUBTASKS, value_subtasks }
	};
}

namespace {

const std::wstring getOrDefault(const TS::STask& stask,
	const std::wstring& key,
	const std::wstring& deflt) {
	auto found = stask.find(key);
	if (found == stask.end()) {
		return deflt;
	} else {
		return found->second;
	}
}

}  // namespace

Task TS::deserialize(const STask& stask) {
	Task::ID id =
		deserializeID(stask.at(KEY_ID));
	Task::Description description =
		deserializeDescription(
			getOrDefault(stask, KEY_DESCRIPTION,
				serializeDescription(Task::DEFAULT_DESCRIPTION)));
	Task::Time deadline =
		deserializeDeadline(
			getOrDefault(stask, KEY_DEADLINE,
				serializeDeadline(Task::DEFAULT_DEADLINE)));
	Task::Priority priority =
		deserializePriority(
			getOrDefault(stask, KEY_PRIORITY,
				serializePriority(Task::DEFAULT_PRIORITY)));
	Task::Dependencies dependencies =
		deserializeDependencies(
			getOrDefault(stask, KEY_DEPENDENCIES,
				serializeDependencies(Task::DEFAULT_DEPENDENCIES)));
	Task::ID parent =
		deserializeParent(
			getOrDefault(stask, KEY_PARENT,
				serializeID(id)));
	Task::Dependencies subtasks =
		deserializeSubtasks(
			getOrDefault(stask, KEY_SUBTASKS,
				serializeSubtasks(Task::DEFAULT_SUBTASKS)));
	return TaskBuilder::get()
		.id(id)
		.description(description)
		.deadline(deadline)
		.dependencies(dependencies)
		.priority(priority)
		.parent(parent)
		.subtasks(subtasks);
}

TS::Value TS::serializeID(const Task::ID id) {
	return boost::lexical_cast<std::wstring>(id);
}

TS::Value TS::serializeDescription(const Task::Description& description) {
	return description;
}

TS::Value TS::serializeDeadline(const Task::Time& deadline) {
	std::wstringstream wss;
	auto date = deadline.date();
	auto time = deadline.time_of_day();
	std::vector<int> fields = {
		date.year(), date.month(), date.day(),
		time.hours(), time.minutes(), time.seconds()
	};
	for (const auto& field : fields) {
		wss << field << VALUE_DELIMITER;
	}
	return wss.str();
}

TS::Value TS::serializePriority(const Task::Priority& priority) {
	return prioStrTable.at(priority);
}

TS::Value TS::serializeDependencies(const Task::Dependencies& dependencies) {
	std::wstringstream ws;
	for (const auto& id : dependencies) {
		ws << TS::serializeID(id);
		ws << TS::VALUE_DELIMITER;
	}
	return ws.str();
}

TS::Value TS::serializeParent(const Task::ID parent) {
	return serializeID(parent);
}

TS::Value TS::serializeSubtasks(const Task::Subtasks& subtasks) {
	return serializeDependencies(subtasks);
}

Task::ID TS::deserializeID(const Value& id) {
	return boost::lexical_cast<Task::ID>(id);
}

Task::Description TS::deserializeDescription(const Value& description) {
	return description;
}

Task::Time TS::deserializeDeadline(const Value& deadline) {
	std::vector<std::int16_t> numbers;
	std::vector<std::wstring> tokens = tokenize(deadline);
	for (const auto& token : tokens) {
		numbers.push_back(boost::lexical_cast<std::int16_t>(token));
	}
	auto year = numbers[0];
	auto month = numbers[1];
	auto day = numbers[2];
	auto hour = numbers[3];
	auto minute = numbers[4];
	auto second = numbers[5];
	return boost::posix_time::ptime(boost::gregorian::date(year, month, day),
		boost::posix_time::time_duration(hour, minute, second));
}

Task::Priority TS::deserializePriority(const Value& priority) {
	return strPrioTable.at(priority);
}

Task::Dependencies TS::deserializeDependencies(const Value& dependencies) {
	Task::Dependencies deps;
	std::vector<std::wstring> tokens = tokenize(dependencies);
	for (const auto& token : tokens) {
		deps.insert(boost::lexical_cast<Task::ID>(token));
	}
	return deps;
}

Task::ID TS::deserializeParent(const Value& parent) {
	return deserializeID(parent);
}

Task::Subtasks TS::deserializeSubtasks(const Value& subtasks) {
	return deserializeDependencies(subtasks);
}

std::vector<std::wstring> TS::tokenize(const std::wstring& input) {
	std::vector<std::wstring> output;
	boost::char_separator<wchar_t> sep(VALUE_DELIMITER.c_str());
	boost::tokenizer<boost::char_separator<wchar_t>,
		std::wstring::const_iterator, std::wstring> tokens(input, sep);
	for (const auto& token : tokens) {
		output.push_back(std::wstring(begin(token), end(token)));
	}
	return output;
}

const std::unordered_map<TS::Value, Task::Priority> TS::strPrioTable = {
	{ VALUE_PRIORITY_NORMAL, Task::Priority::NORMAL },
	{ VALUE_PRIORITY_HIGH, Task::Priority::HIGH },
};

const std::unordered_map<Task::Priority, TS::Value> TS::prioStrTable = {
	{ Task::Priority::NORMAL, VALUE_PRIORITY_NORMAL },
	{ Task::Priority::HIGH, VALUE_PRIORITY_HIGH },
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

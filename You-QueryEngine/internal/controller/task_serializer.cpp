#include "stdafx.h"

#include "../../exception.h"
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
}

const TaskSerializer::Key TaskSerializer::KEY_ID = L"id";
const TaskSerializer::Key TaskSerializer::KEY_DESCRIPTION = L"description";
const TaskSerializer::Key TaskSerializer::KEY_START_TIME = L"start_time";
const TaskSerializer::Key TaskSerializer::KEY_DEADLINE = L"deadline";
const TaskSerializer::Key TaskSerializer::KEY_PRIORITY = L"priority";
const TaskSerializer::Key TaskSerializer::KEY_DEPENDENCIES = L"dependencies";
const TaskSerializer::Key TaskSerializer::KEY_COMPLETED = L"completed";
const TaskSerializer::Key TaskSerializer::KEY_PARENT = L"parent";
const TaskSerializer::Key TaskSerializer::KEY_SUBTASKS = L"subtasks";
const TaskSerializer::Key TaskSerializer::KEY_ATTACHMENT = L"attachment";

const TaskSerializer::Value TaskSerializer::VALUE_PRIORITY_NORMAL = L"normal";
const TaskSerializer::Value TaskSerializer::VALUE_PRIORITY_HIGH = L"high";
const TaskSerializer::Value TaskSerializer::VALUE_DELIMITER = L";";

TaskSerializer::STask TaskSerializer::serialize(const Task& task) {
	Value value_id = serializeID(task.getID());
	Value value_description = serializeDescription(task.getDescription());

	Value value_start_time = serializeTime(task.getStartTime());
	Value value_deadline = serializeTime(task.getDeadline());
	Value value_priority = serializePriority(task.getPriority());
	Value value_dependencies = serializeDependencies(task.getDependencies());
	Value value_completed = serializeCompleted(task.isCompleted());
	Value value_parent = serializeParent(task.getParent());
	Value value_subtasks = serializeSubtasks(task.getSubtasks());
	Value value_attachment = serializeDescription(task.getAttachment());
	return {
		{ KEY_ID, value_id },
		{ KEY_DESCRIPTION, value_description },
		{ KEY_START_TIME, value_start_time },
		{ KEY_DEADLINE, value_deadline },
		{ KEY_PRIORITY, value_priority },
		{ KEY_DEPENDENCIES, value_dependencies },
		{ KEY_COMPLETED, value_completed },
		{ KEY_PARENT, value_parent },
		{ KEY_SUBTASKS, value_subtasks },
		{ KEY_ATTACHMENT, value_attachment }
	};
}

namespace {

const std::wstring getOrDefault(const TaskSerializer::STask& stask,
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

Task TaskSerializer::deserialize(const STask& stask) {
	TaskBuilder sofar = TaskBuilder::get();
	try {
		Task::ID id =
			deserializeID(stask.at(KEY_ID));
		sofar.id(id);
		Task::Description description =
			deserializeDescription(
				getOrDefault(stask, KEY_DESCRIPTION,
					serializeDescription(Task::DEFAULT_DESCRIPTION)));
		sofar.description(description);
		Task::Time startTime =
			deserializeTime(
				getOrDefault(stask, KEY_START_TIME,
					serializeTime(Task::DEFAULT_DEADLINE)));
		sofar.startTime(startTime);
		Task::Time deadline =
			deserializeTime(
				getOrDefault(stask, KEY_DEADLINE,
					serializeTime(Task::DEFAULT_DEADLINE)));
		sofar.deadline(deadline);
		Task::Priority priority =
			deserializePriority(
				getOrDefault(stask, KEY_PRIORITY,
					serializePriority(Task::DEFAULT_PRIORITY)));
		sofar.priority(priority);
		Task::Dependencies dependencies =
			deserializeDependencies(
				getOrDefault(stask, KEY_DEPENDENCIES,
					serializeDependencies(Task::DEFAULT_DEPENDENCIES)));
		sofar.dependencies(dependencies);
		bool completed =
			deserializeCompleted(
				getOrDefault(stask, KEY_COMPLETED,
					serializeCompleted(false)));
		sofar.completed(completed);
		Task::ID parent =
			deserializeParent(
				getOrDefault(stask, KEY_PARENT,
					serializeID(id)));
		sofar.parent(parent);
		Task::Dependencies subtasks =
			deserializeSubtasks(
				getOrDefault(stask, KEY_SUBTASKS,
					serializeSubtasks(Task::DEFAULT_SUBTASKS)));
		sofar.subtasks(subtasks);
		Task::Attachment attachment =
			deserializeDescription(
				getOrDefault(stask, KEY_ATTACHMENT,
					serializeDescription(Task::DEFAULT_ATTACHMENT)));
		sofar.attachment(attachment);
		return sofar;
	} catch (const boost::bad_lexical_cast& e) {
		return sofar;
	}
}


TaskSerializer::Value TaskSerializer::serializeID(const Task::ID id) {
	return boost::lexical_cast<std::wstring>(id);
}

TaskSerializer::Value TaskSerializer::serializeDescription(
	const Task::Description& description) {
	return description;
}

TaskSerializer::Value TaskSerializer::serializeTime(
	const Task::Time& deadline) {
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

TaskSerializer::Value TaskSerializer::serializePriority(
	const Task::Priority& priority) {
	return prioStrTable.at(priority);
}

TaskSerializer::Value TaskSerializer::serializeDependencies(
	const Task::Dependencies& dependencies) {
	std::wstringstream ws;
	for (const auto& id : dependencies) {
		ws << TaskSerializer::serializeID(id);
		ws << TaskSerializer::VALUE_DELIMITER;
	}
	return ws.str();
}

TaskSerializer::Value TaskSerializer::serializeParent(
	const Task::ID parent) {
	return serializeID(parent);
}

TaskSerializer::Value TaskSerializer::serializeCompleted(
	const bool completed) {
	if (completed) {
		return L"true";
	} else {
		return L"false";
	}
}
TaskSerializer::Value TaskSerializer::serializeSubtasks(
	const Task::Subtasks& subtasks) {
	return serializeDependencies(subtasks);
}

Task::ID TaskSerializer::deserializeID(const Value& id) {
	return boost::lexical_cast<Task::ID>(id);
}

Task::Description TaskSerializer::deserializeDescription(
	const Value& description) {
	return description;
}

Task::Time TaskSerializer::deserializeTime(const Value& deadline) {
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

Task::Priority TaskSerializer::deserializePriority(const Value& priority) {
	return strPrioTable.at(priority);
}

Task::Dependencies TaskSerializer::deserializeDependencies(
	const Value& dependencies) {
	Task::Dependencies deps;
	std::vector<std::wstring> tokens = tokenize(dependencies);
	for (const auto& token : tokens) {
		deps.insert(boost::lexical_cast<Task::ID>(token));
	}
	return deps;
}

bool TaskSerializer::deserializeCompleted(const Value& completed) {
	return completed == L"true";
}

Task::ID TaskSerializer::deserializeParent(const Value& parent) {
	return deserializeID(parent);
}

Task::Subtasks TaskSerializer::deserializeSubtasks(const Value& subtasks) {
	return deserializeDependencies(subtasks);
}

std::vector<std::wstring> TaskSerializer::tokenize(const std::wstring& input) {
	std::vector<std::wstring> output;
	boost::char_separator<wchar_t> sep(VALUE_DELIMITER.c_str());
	boost::tokenizer<boost::char_separator<wchar_t>,
		std::wstring::const_iterator, std::wstring> tokens(input, sep);
	for (const auto& token : tokens) {
		output.push_back(std::wstring(begin(token), end(token)));
	}
	return output;
}

const std::unordered_map<TaskSerializer::Value, Task::Priority>
TaskSerializer::strPrioTable = {
	{ VALUE_PRIORITY_NORMAL, Task::Priority::NORMAL },
	{ VALUE_PRIORITY_HIGH, Task::Priority::HIGH },
};

const std::unordered_map<Task::Priority, TaskSerializer::Value>
TaskSerializer::prioStrTable = {
	{ Task::Priority::NORMAL, VALUE_PRIORITY_NORMAL },
	{ Task::Priority::HIGH, VALUE_PRIORITY_HIGH },
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

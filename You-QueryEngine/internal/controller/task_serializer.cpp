//@author A0112054Y
#include "stdafx.h"

#include <boost/date_time/posix_time/time_formatters.hpp>
#include <boost/date_time/posix_time/time_parsers.hpp>
#include "../../exception.h"
#include "task_builder.h"
#include "task_serializer.h"

namespace You {
namespace QueryEngine {
namespace Internal {

using boost::posix_time::ptime;
using boost::gregorian::date;
using boost::gregorian::greg_year_month_day;
using boost::posix_time::time_duration;

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
	Value value_attachment = serializeAttachment(task.getAttachment());
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

Task TaskSerializer::deserialize(const STask& stask) {
	TaskBuilder sofar = TaskBuilder::get();
	try {
		Task::ID id =
			deserializeID(stask.at(KEY_ID));
		sofar.id(id);

		Task::Description description =
			deserializeOrDefault<Task::Description>(
				&TaskSerializer::deserializeDescription,
				stask,
				KEY_DESCRIPTION,
				Task::DEFAULT_DESCRIPTION);
		sofar.description(description);

		Task::Time startTime =
			deserializeOrDefault<Task::Time>(
				&TaskSerializer::deserializeTime,
				stask,
				KEY_START_TIME,
				Task::DEFAULT_START_TIME);
		sofar.startTime(startTime);

		Task::Time deadline =
			deserializeOrDefault<Task::Time>(
				&TaskSerializer::deserializeTime,
				stask,
				KEY_DEADLINE,
				Task::DEFAULT_DEADLINE);
		sofar.deadline(deadline);

		Task::Priority priority =
			deserializeOrDefault<Task::Priority>(
				&TaskSerializer::deserializePriority,
				stask,
				KEY_PRIORITY,
				Task::DEFAULT_PRIORITY);
		sofar.priority(priority);

		Task::Dependencies dependencies =
			deserializeOrDefault<Task::Dependencies>(
				&TaskSerializer::deserializeDependencies,
				stask,
				KEY_DEPENDENCIES,
				Task::DEFAULT_DEPENDENCIES);
		sofar.dependencies(dependencies);

		bool completed =
			deserializeOrDefault<bool>(
				&TaskSerializer::deserializeCompleted,
				stask,
				KEY_COMPLETED,
				false);
		sofar.completed(completed);

		Task::ID parent =
			deserializeOrDefault<Task::ID>(
				&TaskSerializer::deserializeParent,
				stask,
				KEY_PARENT,
				id);
		sofar.parent(parent);

		Task::Dependencies subtasks =
			deserializeOrDefault<Task::Subtasks>(
				&TaskSerializer::deserializeSubtasks,
				stask,
				KEY_SUBTASKS,
				Task::DEFAULT_SUBTASKS);
		sofar.subtasks(subtasks);

		Task::Attachment attachment =
			deserializeOrDefault<Task::Attachment>(
				&TaskSerializer::deserializeAttachment,
				stask,
				KEY_ATTACHMENT,
				Task::DEFAULT_ATTACHMENT);
		sofar.attachment(attachment);

		return sofar;
	} catch (const boost::bad_lexical_cast&) {
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
	std::string stime = boost::posix_time::to_iso_string(deadline);
	std::wstring value(begin(stime), end(stime));
	return value;
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

TaskSerializer::Value TaskSerializer::serializeAttachment(
	const Task::Attachment& attachments) {
	std::wstringstream ws;
	for (const auto& attachment : attachments) {
		ws << attachment;
		ws << TaskSerializer::VALUE_DELIMITER;
	}
	return ws.str();
}

Task::ID TaskSerializer::deserializeID(const Value& id) {
	return boost::lexical_cast<Task::ID>(id);
}

Task::Description TaskSerializer::deserializeDescription(
	const Value& description) {
	return description;
}

Task::Time TaskSerializer::deserializeTime(const Value& deadline) {
	if (deadline.find(VALUE_DELIMITER) != std::wstring::npos) {
		// The user is using the old serialization method.
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
	} else {
		std::string sdeadline(begin(deadline), end(deadline));
		return boost::posix_time::from_iso_string(sdeadline);
	}
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

Task::Attachment TaskSerializer::deserializeAttachment(
	const Value& attachment) {
	return tokenize(attachment);
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

//@author A0097630B
#include "stdafx.h"

#include "task_field.h"

namespace {

/// The description field.
const std::wstring DESCRIPTION_FIELD(L"Description");

/// The priority field.
const std::wstring PRIORITY_FIELD(L"Priority");

/// The start field.
const std::wstring START_FIELD(L"Start");

/// The deadline field.
const std::wstring DEADLINE_FIELD(L"Deadline");

/// The completion field.
const std::wstring COMPLETE_FIELD(L"Complete");

/// The subtasks field.
const std::wstring SUBTASKS_FIELD(L"Subtasks");

/// The dependents of this task.
const std::wstring DEPENDENTS_FIELDS(L"Depending");

/// The attachments field.
const std::wstring ATTACHMENTS_FIELD(L"Attachments");

}  // namespace

namespace You {
namespace NLP {

TaskField operator|(const TaskField& lhs, const TaskField& rhs) {
	return static_cast<TaskField>(
		static_cast<size_t>(lhs) | static_cast<size_t>(rhs)
	);
}

TaskField operator&(const TaskField& lhs, const TaskField& rhs) {
	return static_cast<TaskField>(
		static_cast<size_t>(lhs)& static_cast<size_t>(rhs)
	);
}

std::wostream& operator<<(std::wostream& stream, TaskField priority) {
	return stream << ToString(priority);
}

/// Converts the sorting fields to strings.
std::wstring ToString(const TaskField& field) {
	using TaskField = You::NLP::TaskField;
	switch (field) {
	case TaskField::DESCRIPTION:
		return DESCRIPTION_FIELD;
	case TaskField::PRIORITY:
		return PRIORITY_FIELD;
	case TaskField::START:
		return START_FIELD;
	case TaskField::DEADLINE:
		return DEADLINE_FIELD;
	case TaskField::COMPLETE:
		return COMPLETE_FIELD;
	case TaskField::SUBTASKS:
		return SUBTASKS_FIELD;
	case TaskField::DEPENDENTS:
		return DEPENDENTS_FIELDS;
	case TaskField::ATTACHMENTS:
		return ATTACHMENTS_FIELD;
	case TaskField::NONE:
	default:
		return std::wstring();
	}
}


}  // namespace NLP
}  // namespace You

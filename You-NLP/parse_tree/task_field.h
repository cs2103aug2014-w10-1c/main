//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSE_TREE_TASK_FIELD_H_
#define YOU_NLP_PARSE_TREE_TASK_FIELD_H_

#include <iosfwd>

namespace You {
namespace NLP {

/// The possible fields for tasks.
enum class TaskField {
	/// Not changing anything.
	NONE = 0,

	/// The description field needs to be changed
	DESCRIPTION = 1 << 0,

	/// The priority of the task
	PRIORITY = 1 << 1,

	/// The start field of the task
	START = 1 << 2,

	/// The deadline field
	DEADLINE = 1 << 3,

	/// The task is now complete
	COMPLETE = 1 << 4,

	/// The subtasks of this task.
	SUBTASKS = 1 << 5,

	/// The dependents of this task.
	DEPENDENTS = 1 << 6,

	/// The attachments need to be updated
	ATTACHMENTS = 1 << 7
};

/// Formats the given task priority to stream.
///
/// \param[in] stream The stream to write to.
/// \param[in] priority The priority to display.
/// \return The stream given.
std::wostream& operator<<(std::wostream& stream, TaskField priority);

/// Converts the given TaskField to a string.
///
/// \param[in] field The field to convert.
std::wstring ToString(const TaskField& field);

/// Computes a bitwise OR over the two fields specification flags.
///
/// \param[in] lhs The left hand side of the expression.
/// \param[in] rhs The right hand side of the expression.
/// \return The combined fields from both fields.
TaskField operator|(const TaskField& lhs, const TaskField& rhs);

/// Computes a bitwise AND over the two fields specification flags.
///
/// \param[in] lhs The left hand side of the expression.
/// \param[in] rhs The right hand side of the expression.
/// \return The common fields from both fields.
TaskField operator&(const TaskField& lhs, const TaskField& rhs);

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSE_TREE_TASK_FIELD_H_

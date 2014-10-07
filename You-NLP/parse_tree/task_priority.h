#pragma once
#ifndef YOU_NLP_PARSE_TREE_TASK_PRIORITY_H_
#define YOU_NLP_PARSE_TREE_TASK_PRIORITY_H_

namespace You {
namespace NLP {

/// The possible priorities for tasks.
enum class TaskPriority {
	/// Normal task priority
	NORMAL = 0,

	/// High task priority
	HIGH
};

/// Formats the given task priority to stream.
///
/// \param[in] stream The stream to write to.
/// \param[in] priority The priority to display.
/// \return The stream given.
inline std::wostream& operator<<(std::wostream& stream, TaskPriority priority) {
	return stream << static_cast<int>(priority);
}

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSE_TREE_TASK_PRIORITY_H_

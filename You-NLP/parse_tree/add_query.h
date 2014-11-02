//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSE_TREE_ADD_QUERY_H_
#define YOU_NLP_PARSE_TREE_ADD_QUERY_H_

#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "You-Utils/option.h"
#include "task_priority.h"

namespace You {
namespace NLP {

/// Defines the synthesised value type of an add query.
struct ADD_QUERY {
	/// Equality comparator.
	///
	/// \param[in] rhs The other query object to compare with.
	bool operator==(const ADD_QUERY& rhs) const;

	/// The description of the task.
	std::wstring description;

	/// The priority of the task.
	TaskPriority priority;

	/// The deadline of the task.
	You::Utils::Option<boost::posix_time::ptime> deadline;

	/// The subtasks of the task.
	std::vector<ADD_QUERY> subtasks;

	/// The dependent of this task.
	std::shared_ptr<ADD_QUERY> dependent;
};

/// Defines an output formatter for ADD_QUERY queries.
///
/// \param[in,out] s The stream to write to.
/// \param[in] q The query to display.
/// \return The stream object passed to the formatter.
std::wostream& operator<<(std::wostream& s, const ADD_QUERY& q);

/// String conversion -- mainly useful for unit testing and display.
///
/// \param[in] q The query to convert to a string.
/// \return The string representation of the current query object.
std::wstring ToString(const ADD_QUERY& q);

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSE_TREE_ADD_QUERY_H_

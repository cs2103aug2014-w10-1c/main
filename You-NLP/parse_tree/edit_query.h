//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSE_TREE_EDIT_QUERY_H_
#define YOU_NLP_PARSE_TREE_EDIT_QUERY_H_

#include <string>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "You-Utils/option.h"
#include "task_priority.h"

namespace You {
namespace NLP {

/// Defines the synthesised value type of an edit query.
struct EDIT_QUERY {
	/// A bitfield specifying which fields need to be updated
	///
	/// This can be binary ORed together.
	enum class Fields {
		/// Not changing anything.
		NONE = 0,

		/// The description field needs to be changed
		DESCRIPTION = 1 << 0,

		/// The priority of the task
		PRIORITY = 1 << 1,

		/// The deadline field needs to be changed
		DEADLINE = 1 << 2,

		/// The task is now complete
		COMPLETE = 1 << 3
	};

	/// Equality comparator.
	///
	/// \param[in] rhs The other query object to compare with.
	bool operator==(const EDIT_QUERY& rhs) const;

	/// The task offset which should be changed
	size_t taskID;

	/// The description of the task.
	You::Utils::Option<std::wstring> description;

	/// The priority of the task.
	You::Utils::Option<TaskPriority> priority;

	/// The deadline of the task.
	You::Utils::Option<boost::posix_time::ptime> deadline;

	/// Whether the task is complete
	You::Utils::Option<bool> complete;
};

/// Computes a bitwise OR over the two fields specification flags.
///
/// \param[in] lhs The left hand side of the expression.
/// \param[in] rhs The right hand side of the expression.
/// \return The combined fields from both fields.
EDIT_QUERY::Fields operator|(
	const EDIT_QUERY::Fields& lhs,
	const EDIT_QUERY::Fields& rhs);

/// Computes a bitwise AND over the two fields specification flags.
///
/// \param[in] lhs The left hand side of the expression.
/// \param[in] rhs The right hand side of the expression.
/// \return The common fields from both fields.
EDIT_QUERY::Fields operator&(
	const EDIT_QUERY::Fields& lhs,
	const EDIT_QUERY::Fields& rhs);

/// Defines an output formatter for EDIT_QUERY queries.
///
/// \param[in,out] s The stream to write to.
/// \param[in] q The query to display.
/// \return The stream object passed to the formatter.
std::wostream& operator<<(std::wostream& s, const EDIT_QUERY& q);

/// String conversion -- mainly useful for unit testing and display.
///
/// \param[in] q The query to convert to a string.
/// \return The string representation of the current query object.
std::wstring ToString(const EDIT_QUERY& q);

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSE_TREE_EDIT_QUERY_H_

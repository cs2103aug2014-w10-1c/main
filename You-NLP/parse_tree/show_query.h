//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSE_TREE_SHOW_QUERY_H_
#define YOU_NLP_PARSE_TREE_SHOW_QUERY_H_

#include <string>
#include <vector>

#include "task_field.h"

namespace You {
namespace NLP {

/// Defines the synthesised value type of an edit query.
struct SHOW_QUERY {
	/// Equality comparator.
	///
	/// \param[in] rhs The other query object to compare with.
	bool operator==(const SHOW_QUERY& rhs) const;

	/// The order of the fields.
	std::vector<TaskField> order;
};

/// Defines an output formatter for SHOW_QUERY queries.
///
/// \param[in,out] s The stream to write to.
/// \param[in] q The query to display.
/// \return The stream object passed to the formatter.
std::wostream& operator<<(std::wostream& s, const SHOW_QUERY& q);

/// String conversion -- mainly useful for unit testing and display.
///
/// \param[in] q The query to convert to a string.
/// \return The string representation of the current query object.
std::wstring ToString(const SHOW_QUERY& q);

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSE_TREE_SHOW_QUERY_H_

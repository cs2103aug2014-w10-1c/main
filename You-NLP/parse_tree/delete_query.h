#pragma once
#ifndef YOU_NLP_INTERNAL_PARSE_TREE_DELETE_QUERY_H_
#define YOU_NLP_INTERNAL_PARSE_TREE_DELETE_QUERY_H_

namespace You {
namespace NLP {
namespace Internal {

/// Defines the synthesised value type of a delete query.
struct DELETE_QUERY {
	/// Equality comparator.
	///
	/// \param[in] rhs The other query object to compare with.
	bool operator==(const DELETE_QUERY& rhs) const;

	/// The task offset which should be changed
	size_t taskID;
};

/// Defines an output formatter for DELETE_QUERY queries.
///
/// \param[in,out] s The stream to write to.
/// \param[in] q The query to display.
/// \return The stream object passed to the formatter.
std::wostream& operator<<(std::wostream& s, const DELETE_QUERY& q);

/// String conversion -- mainly useful for unit testing and display.
///
/// \param[in] q The query to convert to a string.
/// \return The string representation of the current query object.
std::wstring ToString(const DELETE_QUERY& q);

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_PARSE_TREE_DELETE_QUERY_H_

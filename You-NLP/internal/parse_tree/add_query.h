#pragma once
#ifndef YOU_NLP_INTERNAL_PARSE_TREE_ADD_QUERY_H_
#define YOU_NLP_INTERNAL_PARSE_TREE_ADD_QUERY_H_

#include <string>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace You {
namespace NLP {
namespace Internal {

/// Defines the synthesised value type of an add query.
struct ADD_QUERY {
	/// Equality comparator.
	///
	/// \param[in] rhs The other query object to compare with.
	bool operator==(const ADD_QUERY& rhs) const;

	/// The description of the task.
	std::wstring description;

	/// The due date of the task.
	boost::posix_time::ptime due;
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

}  // namespace Internal
}  // namespace NLP
}  // namespace You

BOOST_FUSION_ADAPT_STRUCT(
	You::NLP::Internal::ADD_QUERY,
	(std::wstring, description)
	(std::wstring, due)
)

#endif  // YOU_NLP_INTERNAL_PARSE_TREE_ADD_QUERY_H_

#pragma once
#ifndef YOU_NLP_INTERNAL_PARSE_TREE_EDIT_QUERY_H_
#define YOU_NLP_INTERNAL_PARSE_TREE_EDIT_QUERY_H_

#include <string>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace You {
namespace NLP {
namespace Internal {

/// Defines the synthesised value type of an edit query.
struct EDIT_QUERY {
	/// A bitfield specifying which fields need to be updated
	///
	/// This can be binary ORed together.
	enum class FIELDS {
		/// Not changing anything.
		NONE = 0,

		/// The description field needs to be changed
		DESCRIPTION = 1 << 0,

		/// The deadline field needs to be changed
		DUE = 1 << 1,

		/// The task is now complete
		COMPLETE = 1 << 2
	};

	/// Equality comparator.
	///
	/// \param[in] rhs The other query object to compare with.
	bool operator==(const EDIT_QUERY& rhs) const;

	/// The task offset which should be changed
	size_t taskID;

	/// The fields which need to be changed.
	FIELDS fieldsToChange;

	/// The description of the task.
	std::wstring description;

	/// The due date of the task.
	boost::posix_time::ptime due;
};

/// Computes a bitwise OR over the two fields specification flags.
///
/// \param[in] lhs The left hand side of the expression.
/// \param[in] rhs The right hand side of the expression.
/// \return The combined fields from both fields.
EDIT_QUERY::FIELDS operator|(
	const EDIT_QUERY::FIELDS& lhs,
	const EDIT_QUERY::FIELDS& rhs);

/// Computes a bitwise AND over the two fields specification flags.
///
/// \param[in] lhs The left hand side of the expression.
/// \param[in] rhs The right hand side of the expression.
/// \return The common fields from both fields.
EDIT_QUERY::FIELDS operator&(
	const EDIT_QUERY::FIELDS& lhs,
	const EDIT_QUERY::FIELDS& rhs);

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

}  // namespace Internal
}  // namespace NLP
}  // namespace You

BOOST_FUSION_ADAPT_STRUCT(
	You::NLP::Internal::EDIT_QUERY,
	(You::NLP::Internal::EDIT_QUERY::FIELDS, fields)
	(std::wstring, description)
	(std::wstring, due)
)

#endif  // YOU_NLP_INTERNAL_PARSE_TREE_EDIT_QUERY_H_

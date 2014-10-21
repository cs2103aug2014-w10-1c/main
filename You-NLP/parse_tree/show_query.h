//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSE_TREE_SHOW_QUERY_H_
#define YOU_NLP_PARSE_TREE_SHOW_QUERY_H_

#include <string>
#include <vector>

#include "You-Utils/option.h"
#include "task_field.h"

namespace You {
namespace NLP {

/// Defines the synthesised value type of show query.
struct SHOW_QUERY {
	/// The order in which columns are sorted
	enum class Order {
		/// The column will be sorted in an ascending order.
		ASCENDING = 0,

		/// The column will be sorted in a descending order.
		DESCENDING
	};

	/// The predicate for filtering tasks.
	enum class Predicate {
		/// Equality.
		EQUAL,

		/// Inequality
		NOT_EQUAL,

		/// Less than
		LESS_THAN,

		/// Less than or equal
		LESS_THAN_EQ,

		/// Greater than
		GREATER_THAN,

		/// Greater than or equal
		GREATER_THAN_EQ
	};

	/// A predicate for filtering tasks.
	struct FIELD_FILTER {
		/// Equality comparator.
		///
		/// \param[in] rhs The other field order object to compare with.
		bool operator==(const FIELD_FILTER& rhs) const;

		/// The field to filter.
		TaskField field;

		/// The predicate to apply.
		Predicate predicate;

		/// The value to compare against.
		You::Utils::Option<std::wstring> value;
	};

	/// A pair containing the field and the order to sort by.
	struct FIELD_ORDER {
		/// Equality comparator.
		///
		/// \param[in] rhs The other field order object to compare with.
		bool operator==(const FIELD_ORDER& rhs) const;

		/// The field to sort.
		TaskField field;

		/// The order to sort the field by.
		Order order;
	};

	/// Equality comparator.
	///
	/// \param[in] rhs The other query object to compare with.
	bool operator==(const SHOW_QUERY& rhs) const;

	/// The predicates for filtering.
	std::vector<FIELD_FILTER> predicates;

	/// The order of the fields.
	std::vector<FIELD_ORDER> order;
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

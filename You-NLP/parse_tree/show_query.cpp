//@author A0097630B
#include "stdafx.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include "show_query.h"

namespace {

/// The format for displaying an SHOW_QUERY
const boost::wformat STRING_FORMAT(L"Show tasks (criteria %1%, sort by %2%)");

/// The format for displaying a field change.
const boost::wformat SORT_FIELD_FORMAT(L"%1% %2%");

/// The description field.
const std::wstring DESCRIPTION_FIELD(L"Description");

/// The priority field.
const std::wstring PRIORITY_FIELD(L"Priority");

/// The deadline field.
const std::wstring DEADLINE_FIELD(L"Deadline");

/// The completion field.
const std::wstring COMPLETE_FIELD(L"Complete");

/// Sorted ascending
const std::wstring ASCENDING_ORDER(L"ascending");

/// Sorted descending
const std::wstring DESCENDING_ORDER(L"descending");

/// Converts the sorting fields to strings.
std::wstring getFieldAsString(const You::NLP::TaskField& field) {
	using TaskField = You::NLP::TaskField;
	switch (field) {
	case TaskField::DESCRIPTION:
		return DESCRIPTION_FIELD;
	case TaskField::PRIORITY:
		return PRIORITY_FIELD;
	case TaskField::DEADLINE:
		return DEADLINE_FIELD;
	case TaskField::COMPLETE:
		return COMPLETE_FIELD;
	case TaskField::NONE:
	default:
		return std::wstring();
	}
}

std::wstring getSortFieldsAsString(const You::NLP::SHOW_QUERY& q) {
	std::vector<std::wstring> orderStrings;
	orderStrings.reserve(q.order.size());

	std::transform(begin(q.order), end(q.order),
		std::back_inserter(orderStrings),
		[](const You::NLP::SHOW_QUERY::FIELD_ORDER& order) {
			return (boost::wformat(SORT_FIELD_FORMAT) %
				getFieldAsString(order.field) %
				(order.order == You::NLP::SHOW_QUERY::Order::ASCENDING ?
					ASCENDING_ORDER : DESCENDING_ORDER)).str();
		});

	return boost::algorithm::join(orderStrings, L", ");
}

}  // namespace

namespace You {
namespace NLP {

std::wostream& operator<<(std::wostream& s, const SHOW_QUERY& q) {
	std::wstring fields;
	return s << (boost::wformat(STRING_FORMAT) % fields %
		getSortFieldsAsString(q));
}

bool SHOW_QUERY::FIELD_ORDER::operator==(const FIELD_ORDER& rhs) const {
	return field == rhs.field &&
		order == rhs.order;
}

bool SHOW_QUERY::operator==(const SHOW_QUERY& rhs) const {
	return std::equal(order.begin(), order.end(), rhs.order.begin());
}

std::wstring ToString(const SHOW_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace NLP
}  // namespace You

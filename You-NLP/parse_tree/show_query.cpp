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

/// Joins items together
const std::wstring LIST_JOIN_SEPARATOR(L", ");

/// None message
const std::wstring NONE_MESSAGE(L"none");

/// Sorted ascending
const std::wstring ASCENDING_ORDER_MESSAGE(L"ascending");

/// Sorted descending
const std::wstring DESCENDING_ORDER_MESSAGE(L"descending");

/// Gets the predicate fields as a string.
std::wstring getPredicateFieldsAsString(const You::NLP::SHOW_QUERY& q) {
	std::vector<std::wstring> predicateStrings;
	predicateStrings.reserve(q.predicates.size());

	if (predicateStrings.empty()) {
		return NONE_MESSAGE;
	} else {
		return boost::algorithm::join(predicateStrings, LIST_JOIN_SEPARATOR);
	}
}

/// Gets the sorting fields as a string.
std::wstring getSortFieldsAsString(const You::NLP::SHOW_QUERY& q) {
	std::vector<std::wstring> orderStrings;
	orderStrings.reserve(q.order.size());

	std::transform(begin(q.order), end(q.order),
		std::back_inserter(orderStrings),
		[](const You::NLP::SHOW_QUERY::FIELD_ORDER& order) {
			return (boost::wformat(SORT_FIELD_FORMAT) %
				order.field %
				(order.order == You::NLP::SHOW_QUERY::Order::ASCENDING ?
					ASCENDING_ORDER_MESSAGE : DESCENDING_ORDER_MESSAGE)).str();
		});

	if (orderStrings.empty()) {
		return NONE_MESSAGE;
	} else {
		return boost::algorithm::join(orderStrings, LIST_JOIN_SEPARATOR);
	}
}

}  // namespace

namespace You {
namespace NLP {

std::wostream& operator<<(std::wostream& s, const SHOW_QUERY& q) {
	return s << (boost::wformat(STRING_FORMAT) %
		getPredicateFieldsAsString(q) %
		getSortFieldsAsString(q));
}

bool SHOW_QUERY::FIELD_FILTER::operator==(const FIELD_FILTER& rhs) const {
	return field == rhs.field &&
		predicate == rhs.predicate &&
		value == rhs.value;
}

bool SHOW_QUERY::FIELD_ORDER::operator==(const FIELD_ORDER& rhs) const {
	return field == rhs.field &&
		order == rhs.order;
}

bool SHOW_QUERY::operator==(const SHOW_QUERY& rhs) const {
	return predicates.size() == rhs.predicates.size() &&
		std::equal(begin(predicates), end(predicates), begin(rhs.predicates)) &&
		order.size() == rhs.order.size() &&
		std::equal(begin(order), end(order), begin(rhs.order));
}

std::wstring ToString(const SHOW_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace NLP
}  // namespace You

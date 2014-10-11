//@author A0097630B
#include "stdafx.h"
#include <boost/format.hpp>

#include "show_query.h"

namespace {

/// The format for displaying an SHOW_QUERY
const boost::wformat STRING_FORMAT(L"Show tasks (criteria %1%, sort by %2%)");

}  // namespace

namespace You {
namespace NLP {

std::wostream& operator<<(std::wostream& s, const SHOW_QUERY& q) {
	std::wstring criteria;
	std::wstring order;
	return s << (boost::wformat(STRING_FORMAT) % criteria % order);
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

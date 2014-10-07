//@author A0097630B
#include "stdafx.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "add_query.h"

namespace {

/// The format for displaying an ADD_QUERY
const boost::wformat STRING_FORMAT(L"%1% (due %2%, %3% priority)");

}  // namespace

namespace You {
namespace NLP {

std::wostream& operator<<(std::wostream& s, const ADD_QUERY& q) {
	return s << (boost::wformat(STRING_FORMAT) % q.description % (
		q.due ? boost::lexical_cast<std::wstring>(q.due.get()) : L"none"
	) % (
		q.priority == TaskPriority::HIGH ? L"high" : L"normal"
	));
}

bool ADD_QUERY::operator==(const ADD_QUERY& rhs) const {
	return description == rhs.description &&
		priority == rhs.priority &&
		due == rhs.due;
}

std::wstring ToString(const ADD_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace NLP
}  // namespace You

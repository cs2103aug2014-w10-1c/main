//@author A0097630B
#include "stdafx.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "add_query.h"

namespace {

/// The format for displaying an ADD_QUERY
const boost::wformat STRING_FORMAT(L"%1% (start %2%, deadline %3%, "
	L"%4% priority, %5% subtasks, %6% dependents)");

}  // namespace

namespace You {
namespace NLP {

std::wostream& operator<<(std::wostream& s, const ADD_QUERY& q) {
	return s << (boost::wformat(STRING_FORMAT) % q.description % (
		q.start ?
			boost::lexical_cast<std::wstring>(q.start.get()) : L"none"
	) % (
		q.deadline ?
			boost::lexical_cast<std::wstring>(q.deadline.get()) : L"none"
	) % (
		q.priority == TaskPriority::HIGH ? L"high" : L"normal"
	) % (
		q.subtasks.size()
	) % (
		q.dependent ? 1 : 0
	));
}

bool ADD_QUERY::operator==(const ADD_QUERY& rhs) const {
	return description == rhs.description &&
		priority == rhs.priority &&
		start == rhs.start &&
		deadline == rhs.deadline &&
		subtasks.size() == rhs.subtasks.size() &&
		std::equal(begin(subtasks), end(subtasks), begin(rhs.subtasks)) &&
		(
			(!dependent && !rhs.dependent) || (
				dependent && rhs.dependent &&
					*dependent.get() == *rhs.dependent.get()));
}

std::wstring ToString(const ADD_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace NLP
}  // namespace You

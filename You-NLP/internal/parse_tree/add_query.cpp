#include "stdafx.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "add_query.h"

namespace {

/// The format for displaying an ADD_QUERY
const boost::wformat STRING_FORMAT(L"%1% (due %2%)");

}  // namespace

namespace You {
namespace NLP {
namespace Internal {

std::wostream& operator<<(std::wostream& s, const ADD_QUERY& q) {
	return s << q.description << L" (due " << q.due << L")";
}

bool ADD_QUERY::operator==(const ADD_QUERY& rhs) const {
	return description == rhs.description &&
		due == rhs.due;
}

std::wstring ToString(const ADD_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#include "stdafx.h"
#include <boost/lexical_cast.hpp>

#include "add_query.h"

namespace You {
namespace NLP {
namespace Internal {

std::wostream& operator<<(std::wostream& s, const ADD_QUERY& q) {
	return s << q.description << L"(due " << q.due << L")";
}

bool ADD_QUERY::operator==(const ADD_QUERY& rhs) const {
	return description == rhs.description &&
		due == rhs.due;
}

std::wstring ADD_QUERY::toString() const {
	return boost::lexical_cast<std::wstring>(*this);
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

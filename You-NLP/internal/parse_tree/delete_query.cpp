#include "stdafx.h"
#include "delete_query.h"

namespace {

/// The format for displaying an DELETE_QUERY.
const boost::wformat STRING_FORMAT(L"Delete task #%1%");

}  // namespace

namespace You {
namespace NLP {
namespace Internal {

std::wostream& operator<<(std::wostream& s, const DELETE_QUERY& q) {
	return s << boost::wformat(STRING_FORMAT) % q.taskID;
}

bool DELETE_QUERY::operator==(const DELETE_QUERY& rhs) const {
	return taskID == rhs.taskID;
}

std::wstring ToString(const DELETE_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

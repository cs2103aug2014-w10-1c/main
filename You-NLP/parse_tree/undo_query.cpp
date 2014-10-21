//@author A0097630B
#include "stdafx.h"
#include <boost/lexical_cast.hpp>

#include "undo_query.h"

namespace {

/// The format for displaying an UNDO_QUERY
const std::wstring STRING_FORMAT(L"undo");

}  // namespace

namespace You {
namespace NLP {

std::wostream& operator<<(std::wostream& s, const UNDO_QUERY& q) {
	return s << STRING_FORMAT;
}

bool UNDO_QUERY::operator==(const UNDO_QUERY& rhs) const {
	// All undo queries are the same!
	return true;
}

std::wstring ToString(const UNDO_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace NLP
}  // namespace You

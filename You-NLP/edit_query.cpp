#include "stdafx.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "edit_query.h"

using You::NLP::Internal::EDIT_QUERY;

namespace {

/// The format for displaying an EDIT_QUERY.
const boost::wformat STRING_FORMAT(L"Edit task #%1% (%2%)");

/// The format for displaying a field change.
const boost::wformat CHANGE_FIELD_FORMAT(L"%1% => %2%");

/// The description field.
const std::wstring DESCRIPTION_FIELD(L"Description");

std::vector<std::wstring> getChangedFields(const EDIT_QUERY& q) {
	std::vector<std::wstring> fields;
	if (static_cast<size_t>(q.fieldsToChange) &
		static_cast<size_t>(EDIT_QUERY::FIELDS::DESCRIPTION)) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			DESCRIPTION_FIELD %
			q.description).str());
	}

	return fields;
}

}  // namespace

namespace You {
namespace NLP {
namespace Internal {

std::wostream& operator<<(std::wostream& s, const EDIT_QUERY& q) {
	std::vector<std::wstring> fields(getChangedFields(q));
	std::wostringstream fieldsString;
	for (auto i = begin(fields); i != end(fields); ++i) {
		fieldsString << *i;
		if (i != end(fields) - 1) {
			fieldsString << L", ";
		}
	}
	
	return s << boost::wformat(STRING_FORMAT) % q.taskID % fieldsString.str();
}

bool EDIT_QUERY::operator==(const EDIT_QUERY& rhs) const {
	return description == rhs.description &&
		due == rhs.due;
}

std::wstring ToString(const EDIT_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

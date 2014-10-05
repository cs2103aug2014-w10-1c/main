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

/// The due field.
const std::wstring DUE_FIELD(L"Due");

/// Converts the changed fields to a string.
std::vector<std::wstring> getChangedFieldsAsString(const EDIT_QUERY& q) {
	std::vector<std::wstring> fields;
	if ((q.fieldsToChange &
		EDIT_QUERY::FIELDS::DESCRIPTION) != EDIT_QUERY::FIELDS::NONE) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			DESCRIPTION_FIELD %
			q.description).str());
	}

	if ((q.fieldsToChange &
		EDIT_QUERY::FIELDS::DUE) != EDIT_QUERY::FIELDS::NONE) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			DUE_FIELD %
			q.due).str());
	}

	return fields;
}

}  // namespace

namespace You {
namespace NLP {
namespace Internal {

EDIT_QUERY::FIELDS operator|(
	const EDIT_QUERY::FIELDS& lhs,
	const EDIT_QUERY::FIELDS& rhs) {
	return static_cast<EDIT_QUERY::FIELDS>(
		static_cast<size_t>(lhs) | static_cast<size_t>(rhs)
	);
}

EDIT_QUERY::FIELDS operator&(
	const EDIT_QUERY::FIELDS& lhs,
	const EDIT_QUERY::FIELDS& rhs) {
	return static_cast<EDIT_QUERY::FIELDS>(
		static_cast<size_t>(lhs) & static_cast<size_t>(rhs)
	);
}

std::wostream& operator<<(std::wostream& s, const EDIT_QUERY& q) {
	std::vector<std::wstring> fields(getChangedFieldsAsString(q));
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
	if (fieldsToChange != rhs.fieldsToChange ||
		taskID != rhs.taskID) {
		return false;
	}

	if ((fieldsToChange & FIELDS::DESCRIPTION) != FIELDS::NONE) {
		if (description != rhs.description) {
			return false;
		}
	}

	if ((fieldsToChange & FIELDS::DUE) != FIELDS::NONE) {
		if (due != rhs.due) {
			return false;
		}
	}

	return true;
}

std::wstring ToString(const EDIT_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

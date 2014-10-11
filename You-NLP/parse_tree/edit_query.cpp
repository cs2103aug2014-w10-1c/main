//@author A0097630B
#include "stdafx.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>

#include "edit_query.h"

using You::NLP::EDIT_QUERY;

namespace {

/// The format for displaying an EDIT_QUERY.
const boost::wformat STRING_FORMAT(L"Edit task #%1% (%2%)");

/// The format for displaying a field change.
const boost::wformat CHANGE_FIELD_FORMAT(L"%1% => %2%");

/// The description field.
const std::wstring DESCRIPTION_FIELD(L"Description");

/// The priority field.
const std::wstring PRIORITY_FIELD(L"Priority");

/// The deadline field.
const std::wstring DEADLINE_FIELD(L"Deadline");

/// The completion field.
const std::wstring COMPLETE_FIELD(L"Complete");

/// Converts the changed fields to a string.
std::vector<std::wstring> getChangedFieldsAsString(const EDIT_QUERY& q) {
	std::vector<std::wstring> fields;
	if (q.description) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			DESCRIPTION_FIELD %
			q.description.get()).str());
	}

	if (q.priority) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			PRIORITY_FIELD %
			q.priority.get()).str());
	}

	if (q.deadline) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			DEADLINE_FIELD %
			q.deadline.get()).str());
	}

	if (q.complete) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			COMPLETE_FIELD %
			q.complete.get()).str());
	}
	return fields;
}

}  // namespace

namespace You {
namespace NLP {

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
	if (taskID != rhs.taskID) {
		return false;
	}

	return description == rhs.description &&
		priority == rhs.priority &&
		deadline == rhs.deadline &&
		complete == rhs.complete;
}

std::wstring ToString(const EDIT_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace NLP
}  // namespace You

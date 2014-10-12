//@author A0097630B
#include "stdafx.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "task_field.h"
#include "edit_query.h"

using You::NLP::EDIT_QUERY;

namespace {

/// The format for displaying an EDIT_QUERY.
const boost::wformat STRING_FORMAT(L"Edit task #%1% (%2%)");

/// The format for displaying a field change.
const boost::wformat CHANGE_FIELD_FORMAT(L"%1% => %2%");

/// Converts the changed fields to a string.
std::vector<std::wstring> getChangedFieldsAsString(const EDIT_QUERY& q) {
	using You::NLP::TaskField;

	std::vector<std::wstring> fields;
	if (q.description) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::DESCRIPTION %
			q.description.get()).str());
	}

	if (q.priority) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::PRIORITY %
			q.priority.get()).str());
	}

	if (q.deadline) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::DEADLINE %
			q.deadline.get()).str());
	}

	if (q.complete) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::COMPLETE %
			q.complete.get()).str());
	}
	return fields;
}

}  // namespace

namespace You {
namespace NLP {

std::wostream& operator<<(std::wostream& s, const EDIT_QUERY& q) {
	return s << boost::wformat(STRING_FORMAT) % q.taskID %
		boost::algorithm::join(getChangedFieldsAsString(q), L", ");
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

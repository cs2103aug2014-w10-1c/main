//@author A0097630B
#include "stdafx.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

#include "task_field.h"
#include "edit_query.h"

using You::NLP::EDIT_QUERY;

namespace You {
namespace NLP {
namespace {

/// The format for displaying an EDIT_QUERY.
const boost::wformat STRING_FORMAT(L"Edit task #%1% (%2%)");

/// The format for displaying a field change.
const boost::wformat CHANGE_FIELD_FORMAT(L"%1% => %2%");

/// Converts the attachment actions to a string.
std::vector<std::wstring> getAttachmentActionsAsString(
	const std::vector<EDIT_QUERY::ATTACHMENT_ACTION>& q) {
	std::vector<std::wstring> result;
	std::transform(begin(q), end(q), std::back_inserter(result),
		[](const EDIT_QUERY::ATTACHMENT_ACTION& a) {
			return boost::lexical_cast<std::wstring>(a);
		});

	return result;
}

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

	if (q.start) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::START %
			q.start.get()).str());
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

	if (q.childTask) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::SUBTASKS %
			q.childTask.get()).str());
	}

	if (q.dependingTask) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::DEPENDENTS %
			q.dependingTask.get()).str());
	}

	if (!q.attachments.empty()) {
		fields.emplace_back(
			(boost::wformat(CHANGE_FIELD_FORMAT) %
			TaskField::ATTACHMENTS %
			boost::algorithm::join(getAttachmentActionsAsString(q.attachments),
				L", ")).str());
	}
	return fields;
}

}  // namespace

std::wostream& operator<<(std::wostream& s, const EDIT_QUERY& q) {
	return s << boost::wformat(STRING_FORMAT) % q.taskID %
		boost::algorithm::join(getChangedFieldsAsString(q), L", ");
}

std::wostream& operator<<(std::wostream& s,
	const EDIT_QUERY::ATTACHMENT_ACTION& a) {
	return s <<
		(a.add ? L"add" : L"remove") << L' ' <<
		a.path;
}

bool EDIT_QUERY::operator==(const EDIT_QUERY& rhs) const {
	if (taskID != rhs.taskID) {
		return false;
	}

	return description == rhs.description &&
		priority == rhs.priority &&
		deadline == rhs.deadline &&
		complete == rhs.complete &&
		childTask == rhs.childTask &&
		dependingTask == rhs.dependingTask &&
		attachments.size() == rhs.attachments.size() &&
		std::equal(
			begin(attachments),
			end(attachments),
			begin(rhs.attachments));
}

bool EDIT_QUERY::ATTACHMENT_ACTION::operator==(
	const ATTACHMENT_ACTION& rhs) const {
	return add == rhs.add &&
		path == rhs.path;
}

std::wstring ToString(const EDIT_QUERY& q) {
	return boost::lexical_cast<std::wstring>(q);
}

}  // namespace NLP
}  // namespace You

//@author A0097630B
#include "stdafx.h"
#include "date_time_parser.h"
#include "query_parser.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace phoenix = boost::phoenix;

ADD_QUERY QueryParser::constructAddQuery(ParserCharEncoding::char_type lexeme,
	const ADD_QUERY& query) {
	ADD_QUERY result(query);
	result.description.insert(result.description.begin(), lexeme);

	return result;
}

ADD_QUERY QueryParser::constructAddQueryWithPriority(const ADD_QUERY& query) {
	ADD_QUERY result(query);
	result.priority = TaskPriority::HIGH;
	return result;
}

ADD_QUERY QueryParser::constructAddQueryWithDeadline(const LexemeType& lexeme) {
	return ADD_QUERY {
		std::wstring(),
		TaskPriority::NORMAL,
		DateTimeParser::parse(std::wstring(lexeme.begin(), lexeme.end()))
	};
}

ADD_QUERY QueryParser::constructAddQueryWithOptionalDeadline(
	const boost::optional<ADD_QUERY>& query) {
	if (query) {
		return query.get();
	} else {
		return ADD_QUERY {
		};
	}
}

EDIT_QUERY QueryParser::constructEditQuery(
	const size_t offset,
	const EDIT_QUERY& query) {
	EDIT_QUERY result(query);
	result.taskID = offset;

	return result;
}

EDIT_QUERY QueryParser::constructEditQueryNullary(EDIT_QUERY::Fields field) {
	EDIT_QUERY result;

	switch (field) {
	case EDIT_QUERY::Fields::COMPLETE:
		result.complete = true;
		break;
	default:
		assert(false);
	}

	return result;
}

EDIT_QUERY QueryParser::constructEditQueryUnary(
	EDIT_QUERY::Fields field,
	const LexemeType& newValue) {
	StringType newStringValue(newValue.begin(), newValue.end());
	EDIT_QUERY result;

	switch (field) {
	case EDIT_QUERY::Fields::DESCRIPTION:
		result.description = newStringValue;
		break;
	case EDIT_QUERY::Fields::DEADLINE:
		result.deadline = DateTimeParser::parse(newStringValue);
		break;
	default:
		assert(false);
	}

	return result;
}

EDIT_QUERY QueryParser::constructEditQueryPriority(TaskPriority priority) {
	EDIT_QUERY result;
	result.priority = priority;

	return result;
}

DELETE_QUERY QueryParser::constructDeleteQuery(const size_t offset) {
	return DELETE_QUERY {
		offset
	};
}


}  // namespace NLP
}  // namespace You

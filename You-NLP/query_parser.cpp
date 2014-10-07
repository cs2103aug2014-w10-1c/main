//@author A0097630B
#include "stdafx.h"
#include "date_time_parser.h"
#include "exceptions/parse_error_exception.h"
#include "query_parser.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;
namespace phoenix = boost::phoenix;

QUERY QueryParser::parse(const QueryParser::StringType& string) {
	QUERY result;
	bool success = qi::phrase_parse(
		begin(string),
		end(string),
		QueryParser(),
		ParserSkipperType(),
		result);

	if (success) {
		return result;
	} else {
		throw ParserException();
	}
}

QueryParser::QueryParser() : QueryParser::base_type(start) {
	start %= (
		(qi::lit(L'/') > explicitCommand) |
		addCommand
	) >> qi::eoi;
	start.name("start");

	explicitCommand %= (
		(qi::lit(L"add") >> addCommand) |
		(qi::lit(L"edit") >> editCommand) |
		(qi::lit(L"delete") >> deleteCommand)
	);
	explicitCommand.name("explicitCommand");

	#pragma region Adding tasks
	addCommand = (
		ParserCharTraits::char_ >> addCommandDescription
	)[qi::_val = phoenix::bind(&constructAddQuery, qi::_1, qi::_2)];
	addCommand.name("addCommand");

	addCommandDescription = (
		ParserCharTraits::char_ >> addCommandDescriptionTail
	)[qi::_val = phoenix::bind(&constructAddQuery, qi::_1, qi::_2)];
	addCommandDescription.name("addCommandDescription");

	addCommandDescriptionTail %= (
		(qi::omit[*ParserCharTraits::blank] >> addCommandPriority) |
		addCommandDescription
	);
	addCommandDescriptionTail.name("addCommandDescriptionTail");

	addCommandPriority %= qi::skip(ParserCharTraits::blank)[(
		(
			qi::lit('!') >> addCommandDeadlineOptional
		)[qi::_val = phoenix::bind(&constructAddQueryWithPriority, qi::_1)] |
		addCommandDeadlineOptional
	)];
	addCommandPriority.name("addCommandPriority");

	addCommandDeadline = (
		(qi::lit("by") | qi::lit("before")) >>
		qi::lexeme[+ParserCharTraits::char_]
	)[qi::_val = phoenix::bind(
		&QueryParser::constructAddQueryWithDeadline, qi::_1)];
	addCommandDeadline.name("addCommandDeadline");

	addCommandDeadlineOptional = (
		addCommandDeadline || qi::eoi
	)[qi::_val = phoenix::bind(&constructAddQueryWithOptionalDeadline, qi::_1)];
	
	#pragma endregion

	#pragma region Editing tasks
	editCommand = (
		qi::uint_ >> qi::lit(L"set") >> editCommandRule
	)[qi::_val = phoenix::bind(&QueryParser::constructEditQuery,
		qi::_1, qi::_2)];
	editCommand.name("editCommand");

	editCommandRule %= (
		editCommandRuleNullary | 
		editCommandRuleUnary |
		editCommandRulePriorities
	);
	editCommandRule.name("editCommandRule");

	editCommandRuleNullary = (
		editCommandFieldsNullary
	)[qi::_val = phoenix::bind(&constructEditQueryNullary, qi::_1)];
	editCommandRuleNullary.name("editCommandRuleNullary");

	editCommandRuleUnary = (
		editCommandFieldsUnary >> *ParserCharTraits::char_
		)[qi::_val = phoenix::bind(&constructEditQueryUnary, qi::_1, qi::_2)];
	editCommandRuleUnary.name("editCommandRuleUnary");

	editCommandRulePriorities = (
		qi::lit(L"priority") >> editCommandFieldPriorities
	)[qi::_val = phoenix::bind(&constructEditQueryPriority, qi::_1)];
	editCommandRulePriorities.name("editCommandRulePriorities");

	editCommandFieldsUnary.add
		(L"description", EDIT_QUERY::Fields::DESCRIPTION)
		(L"deadline", EDIT_QUERY::Fields::DEADLINE);
	editCommandFieldsUnary.name("editCommandFieldsUnary");

	editCommandFieldsNullary.add
		(L"done", EDIT_QUERY::Fields::COMPLETE)
		(L"complete", EDIT_QUERY::Fields::COMPLETE);
	editCommandFieldsNullary.name("editCommandFieldsNullary");

	editCommandFieldPriorities.add
		(L"normal", TaskPriority::NORMAL)
		(L"high", TaskPriority::HIGH);
	editCommandFieldPriorities.name("editCommandFieldPriorities");
	#pragma endregion

	#pragma region Deleting tasks
	deleteCommand = (
		qi::uint_
	)[qi::_val = phoenix::bind(&QueryParser::constructDeleteQuery, qi::_1)];
	deleteCommand.name("deleteCommand");
	#pragma endregion

	qi::on_error<qi::fail>(start,
		phoenix::bind(&QueryParser::onFailure, qi::_1, qi::_2, qi::_3, qi::_4));
}

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

void QueryParser::onFailure(
	ParserIteratorType /*begin*/,
	ParserIteratorType end,
	ParserIteratorType errorPos,
	const spirit::info& message) {
	StringType lexeme(errorPos, end);
	throw ParseErrorException(message, lexeme);
}

}  // namespace NLP
}  // namespace You

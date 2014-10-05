#include "stdafx.h"
#include "You-QueryEngine/api.h"
#include "date_time_parser.h"
#include "../exceptions/parse_error_exception.h"
#include "query_parser.h"

namespace You {
namespace NLP {
namespace Internal {

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
	);
	start.name("start");

	explicitCommand %= (
		(qi::lit(L"add") >> addCommand) |
		(qi::lit(L"edit") >> editCommand)
	);
	explicitCommand.name("explicitCommand");

	#pragma region Adding tasks
	addCommand %= (
		addCommandWithDeadline |
		addCommandWithDescription
	);
	addCommand.name("addCommand");

	addCommandWithDescription = (
		qi::lexeme[+ParserCharTraits::char_]
	)[qi::_val = phoenix::bind(&QueryParser::constructAddQuery, qi::_1)];
	addCommandWithDescription.name("addCommandWithDescription");

	addCommandWithDeadline = (
		addCommandWithDeadlineTail |
		(ParserCharTraits::char_ >> addCommandWithDeadline)
	)[qi::_val = phoenix::bind(
		&QueryParser::constructAddQueryWithDeadline,
		qi::_1)];
	addCommand.name("addCommandWithDeadline");

	addCommandWithDeadlineTail = (
		ParserCharTraits::char_
		>> (qi::lit("by") | qi::lit("before"))
		>> qi::lexeme[+ParserCharTraits::char_]
	)[qi::_val = phoenix::bind(
		&QueryParser::constructAddQueryWithDeadlineTail,
		qi::_1, qi::_2)];
	#pragma endregion

	#pragma region Editing tasks
	editCommand = (
		qi::uint_ >> qi::lit(L"set") >> editCommandFields >>
			*ParserCharTraits::char_
	)[qi::_val = phoenix::bind(&QueryParser::constructEditQuery,
		qi::_1, qi::_2, qi::_3)];
	editCommand.name("editCommand");

	editCommandFields.add
		(L"description", EDIT_QUERY::FIELDS::DESCRIPTION)
		(L"due", EDIT_QUERY::FIELDS::DUE)
		(L"done", EDIT_QUERY::FIELDS::COMPLETE)
		(L"complete", EDIT_QUERY::FIELDS::COMPLETE);
	editCommandFields.name("editCommandFields");
	#pragma endregion

	qi::on_error<qi::fail>(start,
		phoenix::bind(&QueryParser::onFailure, qi::_1, qi::_2, qi::_3, qi::_4));
}

ADD_QUERY QueryParser::constructAddQuery(const LexemeType& lexeme) {
	return ADD_QUERY {
		std::wstring(lexeme.begin(), lexeme.end()),
		QueryEngine::Task::DEFAULT_DEADLINE
	};
}

ADD_QUERY QueryParser::constructAddQueryWithDeadline(
	const boost::variant<
		ADD_QUERY,
		boost::fusion::vector<ParserCharEncoding::char_type, ADD_QUERY>
	>& lexeme) {
	typedef ADD_QUERY HeadType;
	const HeadType* head = boost::get<HeadType>(&lexeme);

	typedef boost::fusion::vector<
			ParserCharEncoding::char_type,
			ADD_QUERY
		> TailType;
	const TailType* tail = boost::get<TailType>(&lexeme);

	if (head) {
		return *head;
	} else {
		ParserCharEncoding::char_type char_ = boost::fusion::at_c<0>(*tail);
		const ADD_QUERY& query(boost::fusion::at_c<1>(*tail));
		return ADD_QUERY {
			std::wstring(1, char_) + query.description,
			query.due
		};
	}
}

ADD_QUERY QueryParser::constructAddQueryWithDeadlineTail(
	const ParserCharEncoding::char_type c, const LexemeType& lexeme) {
	return ADD_QUERY {
		std::wstring(1, c),
		DateTimeParser::parse(std::wstring(lexeme.begin(), lexeme.end()))
	};
}

EDIT_QUERY QueryParser::constructEditQuery(
	const size_t offset,
	EDIT_QUERY::FIELDS field,
	const LexemeType& newValue) {
	EDIT_QUERY result {
		offset,
		field
	};
	StringType newStringValue(newValue.begin(), newValue.end());

	switch (field) {
	case EDIT_QUERY::FIELDS::DESCRIPTION:
		result.description = newStringValue;
		break;
	case EDIT_QUERY::FIELDS::DUE:
		result.due = DateTimeParser::parse(newStringValue);
		break;
	case EDIT_QUERY::FIELDS::NONE:
	case EDIT_QUERY::FIELDS::COMPLETE:
	default:
		break;
	}

	return result;
}

void QueryParser::onFailure(
	ParserIteratorType /*begin*/,
	ParserIteratorType end,
	ParserIteratorType errorPos,
	const spirit::info& message) {
	StringType lexeme(errorPos, end);
	throw ParseErrorException(message, lexeme);
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

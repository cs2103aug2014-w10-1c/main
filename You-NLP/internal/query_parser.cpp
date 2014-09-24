#include "stdafx.h"
#include "query_parser.h"
#include "../exceptions/parse_error_exception.h"

namespace You {
namespace NLP {
namespace Internal {

namespace qi = boost::spirit::qi;
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

	explicitCommand %=
		(qi::lit(L"add") >> addCommand);
	explicitCommand.name("explicitCommand");

	addCommand = (
		qi::lexeme[+ParserCharTraits::char_]
	)[qi::_val = phoenix::bind(&QueryParser::constructAddQuery, qi::_1)];
	addCommand.name("addCommand");

	qi::on_error<qi::fail>(start,
		phoenix::bind(&QueryParser::onFailure, qi::_1, qi::_2, qi::_3, qi::_4));
}

ADD_QUERY QueryParser::constructAddQuery(const LexemeType& lexeme) {
	return ADD_QUERY {
		std::wstring(lexeme.begin(), lexeme.end()),
		std::wstring()
	};
}

void QueryParser::onFailure(ParserIteratorType begin, ParserIteratorType end,
	ParserIteratorType errorPos, const spirit::info& message) {
	StringType lexeme(errorPos, end);
	throw ParseErrorException(message, lexeme);
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

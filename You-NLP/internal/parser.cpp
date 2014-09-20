#include "stdafx.h"
#include "parser.h"
#include "../exceptions/parse_error_exception.h"

namespace You {
namespace NLP {
namespace Internal {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

QUERY Parser::parse(const Parser::StringType& string) {
	QUERY result;
	bool success = qi::phrase_parse(
		begin(string),
		end(string),
		Parser(),
		ParserSkipperType(),
		result);

	if (success) {
		return result;
	} else {
		throw ParserException();
	}
}

Parser::Parser() : Parser::base_type(start) {
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
	)[qi::_val = phoenix::bind(&Parser::constructAddQuery, qi::_1)];
	addCommand.name("addCommand");

	qi::on_error<qi::fail>(start,
		phoenix::bind(&Parser::onFailure, qi::_1, qi::_2, qi::_3, qi::_4));
}

ADD_QUERY Parser::constructAddQuery(const LexemeType& lexeme) {
	return ADD_QUERY {
		std::wstring(lexeme.begin(), lexeme.end()),
		std::wstring()
	};
}

void Parser::onFailure(ParserIteratorType begin, ParserIteratorType end,
	ParserIteratorType errorPos, const spirit::info& message) {

	throw ParseErrorException(message, StringType(errorPos, end));
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

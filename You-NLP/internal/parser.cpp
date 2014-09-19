#include "stdafx.h"
#include "parser.h"

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
		// TODO(lowjoel): Raise an exception.
		assert(false);
	}

	return result;
}

Parser::Parser() : Parser::base_type(start) {
	start %= (
		(qi::lit(L"/") >> explicitCommand) |
		addCommand
	);

	explicitCommand %=
		(qi::lit(L"add") >> addCommand) |
		(qi::lit(L";")[qi::_val = ADD_QUERY()]);

	addCommand = (
		qi::lexeme[+ParserCharTraits::char_]
	)[qi::_val = phoenix::bind(&Parser::constructAddQuery, qi::_1)];
}

ADD_QUERY Parser::constructAddQuery(const LexemeType& lexeme) {
	return ADD_QUERY {
		std::wstring(lexeme.begin(), lexeme.end()),
		std::wstring()
	};
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#include "stdafx.h"
#include "parser.h"

namespace You {
namespace NLP {
namespace Internal {

namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

Parser::Parser() : Parser::base_type(start) {
	start = (
		(qi::lit(L"/") >> explicitCommand) |
		addCommand
	)[qi::_val = phoenix::bind(&Parser::combineStart, qi::_1)]
	;

	explicitCommand %=
		(qi::lit(L"add") >> addCommand) |
		(qi::lit(L";")[qi::_val = ADD_QUERY()])
	;

	addCommand = (
		qi::lexeme[+ParserCharTraits::char_]
	)[qi::_val = phoenix::bind(&Parser::constructAddQuery, qi::_1)]
	;
}

QUERY Parser::combineStart(boost::variant<QUERY, ADD_QUERY>& nonterminal) {
	struct StartVisitor : public boost::static_visitor<QUERY> {
		QUERY operator()(QUERY& q) {
			return q;
		}

		QUERY operator()(ADD_QUERY& q) {
			return q;
		}
	} visitor;

	return boost::apply_visitor(visitor, nonterminal);
};

ADD_QUERY Parser::constructAddQuery(const LexemeType& lexeme) {
	return ADD_QUERY {
		std::wstring(lexeme.begin(), lexeme.end()),
		std::wstring()
	};
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

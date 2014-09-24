#pragma once
#ifndef YOU_NLP_INTERNAL_QUERY_PARSER_H_
#define YOU_NLP_INTERNAL_QUERY_PARSER_H_

#include <boost/spirit/include/qi.hpp>

#include "parse_tree.h"

namespace You {
namespace NLP {
namespace Internal {

namespace spirit = boost::spirit;
namespace qi = spirit::qi;

/// Import the entire character traits namespace from Spirit.
namespace ParserCharTraits { using namespace qi::standard_wide; } // NOLINT

/// The type of one character in the lexer stream.
typedef boost::spirit::char_encoding::standard_wide ParserCharEncoding;

/// The type of the parser iterator.
typedef std::wstring::const_iterator ParserIteratorType;

/// The type of the parser skipper.
typedef ParserCharTraits::space_type ParserSkipperType;

/// The query parser that recognises our input syntax.
class QueryParser : public qi::grammar<
	ParserIteratorType,
	QUERY(),
	ParserSkipperType> {
public:
	/// The type of the iterator used in this grammar.
	typedef ParserIteratorType IteratorType;

	/// The type of the skipper used in this grammar.
	typedef ParserSkipperType SkipperType;

	/// The type of the lexeme buffer provided in a lexing semantic action.
	typedef std::vector<ParserCharEncoding::char_type> LexemeType;

	/// The type of input strings accepted by this parser.
	typedef std::basic_string<ParserCharEncoding::char_type> StringType;

public:
	QueryParser();

	/// Parses the given input string into a query.
	///
	/// \param[in] string The string to parse.
	/// \return The query tree representing the given query.
	static QUERY parse(const StringType& string);

private:
	/// Process the nonterminal returned from the add query rule, converting it
	/// to an appropriate \ref ADD_QUERY type.
	///
	/// \param[in] lexeme The lexeme from the parser.
	/// \return The synthesised value for the \ref addCommand rule.
	static ADD_QUERY constructAddQuery(const LexemeType& lexme);

	/// Handles failures in parsing. This raises a \ref ParseErrorException.
	///
	/// \exception ParseErrorException The exception representing the parse
	///                                error.
	static void onFailure(ParserIteratorType begin, ParserIteratorType end,
		ParserIteratorType errorPos, const spirit::info& message);

private:
	/// The start rule.
	start_type start;

	/// Explicit command rule.
	qi::rule<IteratorType, QUERY(), SkipperType> explicitCommand;

	/// Add command rule.
	qi::rule<IteratorType, ADD_QUERY(), SkipperType> addCommand;
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_QUERY_PARSER_H_

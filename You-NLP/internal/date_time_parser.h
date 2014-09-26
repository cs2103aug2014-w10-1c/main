#pragma once
#ifndef YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_
#define YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include "parser.h"

namespace You {
namespace NLP {
namespace Internal {

/// The query parser that recognises our input syntax.
class DateTimeParser : public boost::spirit::qi::grammar<
	ParserIteratorType,
	boost::posix_time::ptime(),
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
	/// Parses the given input string into date.
	///
	/// \param[in] string The string to parse.
	/// \return The date/time representing the string.
	static boost::posix_time::ptime parse(const StringType& string);

private:
	DateTimeParser();
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_

#pragma once
#ifndef YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_
#define YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include "parser.h"

namespace You {
namespace NLP {
namespace Internal {
namespace UnitTests { class DateTimeParserTests; }

/// The query parser that recognises our input syntax.
class DateTimeParser : public boost::spirit::qi::grammar<
	ParserIteratorType,
	boost::posix_time::ptime(),
	ParserSkipperType> {
public:
	friend class UnitTests::DateTimeParserTests;

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

	/// Parses the given two digit year into a full year.
	///
	/// This is a semantic action for the \ref twoDigitYear rule.
	static int parseTwoDigitYear(
		ParserCharEncoding::char_type _1,
		ParserCharEncoding::char_type _2);

	/// Parses the given two-digit year into a full year according to system
	/// settings (on Windows) or using strptime conversion rules.
	///
	/// \param[in] year The year to convert. This must be between 0 and 99
	///                 inclusive.
	/// \return The actual year referenced.
	static int parseTwoDigitYearNative(int year);

private:
	/// The start rule.
	start_type start;

	/// Parsing years: this handles two-digit years.
	boost::spirit::qi::rule<IteratorType, int(), SkipperType> twoDigitYear;

	/// Parsing years: this handles four-digit years.
	boost::spirit::qi::rule<IteratorType, int(), SkipperType> fullYear;
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_

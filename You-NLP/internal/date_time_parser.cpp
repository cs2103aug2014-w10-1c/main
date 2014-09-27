#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "../exceptions/parser_exception.h"
#include "date_time_parser.h"

namespace You {
namespace NLP {
namespace Internal {

using ptime = boost::posix_time::ptime;
using date = boost::gregorian::date;
namespace qi = boost::spirit::qi;
namespace phoenix = boost::phoenix;

ptime DateTimeParser::parse(const StringType& string) {
	ptime result;
	bool success = qi::phrase_parse(
		begin(string),
		end(string),
		DateTimeParser(),
		ParserSkipperType(),
		result);

	if (success) {
		return result;
	} else {
		throw ParserException();
	}
}

DateTimeParser::DateTimeParser() : DateTimeParser::base_type(start) {
	start = (
		twoDigitYear |
		fullYear
	)[qi::_val = phoenix::construct<ptime>(
		phoenix::construct<date>(qi::_1, boost::gregorian::Dec, 25),
		phoenix::val(boost::posix_time::hours(3)))];

	twoDigitYear = (
		qi::char_ >> qi::char_
	)[qi::_val = phoenix::bind(
		&DateTimeParser::parseTwoDigitYear,
		qi::_1,
		qi::_2)];

	fullYear %=
		qi::int_;
}

int DateTimeParser::parseTwoDigitYear(
	ParserCharEncoding::char_type _1,
	ParserCharEncoding::char_type _2) {
	StringType string({ _1, _2 });
	int twoDigitYear = boost::lexical_cast<int>(string);

	return parseTwoDigitYearNative(twoDigitYear);
}

boost::gregorian::greg_year DateTimeParser::parseTwoDigitYearNative(int year) {
	assert(year >= 0);
	assert(year <= 99);

#ifdef _WIN32
	// TODO(lowjoel): get the actual Windows implementation, from the .NET
	// framework
	return static_cast<boost::gregorian::greg_year>(2000 + year);
#else
	if (year >= 69) {
		return 1900 + year;
	} else {
		return 2000 + year;
	}
#endif
}

}  // namespace Internal
}  // namespace NLP
}  // namespace You

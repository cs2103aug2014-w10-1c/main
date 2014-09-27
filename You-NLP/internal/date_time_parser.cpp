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
		year
	)[qi::_val = phoenix::construct<ptime>(
		phoenix::construct<date>(qi::_1, boost::gregorian::Dec, 25),
		phoenix::val(boost::posix_time::hours(3)))];

	year = (
		+ParserCharTraits::digit
	)[qi::_val = phoenix::bind(&DateTimeParser::parseFuzzyYear, qi::_1)];
}

int16_t DateTimeParser::parseFuzzyYear(
	const std::vector<ParserCharEncoding::char_type>& chars) {
	int16_t value = boost::lexical_cast<int16_t>(
		StringType(chars.begin(), chars.end()));
	if (chars.size() == 2) {
		return parseTwoDigitYear(value);
	} else {
		return value;
	}
}

int16_t DateTimeParser::parseTwoDigitYear(int16_t year) {
	assert(year >= 0);
	assert(year <= 99);

#ifdef _WIN32
	// TODO(lowjoel): get the actual Windows implementation, from the .NET
	// framework
	return 2000 + year;
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

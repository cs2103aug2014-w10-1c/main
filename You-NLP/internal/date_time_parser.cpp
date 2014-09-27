#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include <boost/fusion/sequence.hpp>
#include "../exceptions/parser_exception.h"
#include "date_time_parser.h"

namespace You {
namespace NLP {
namespace Internal {

using ptime = boost::posix_time::ptime;
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
	start %= (
		date[qi::_val = phoenix::construct<DateTime>(
			qi::_1,
			boost::posix_time::hours(0)
		)]
	) >> qi::eoi;

	#pragma region Primitive date component parsers
	year = (
		+ParserCharTraits::digit
	)[qi::_val = phoenix::bind(&DateTimeParser::parseFuzzyYear, qi::_1)];

	
	monthNames.add
		(L"jan", boost::gregorian::Jan)
		(L"feb", boost::gregorian::Feb)
		(L"mar", boost::gregorian::Mar)
		(L"apr", boost::gregorian::Apr)
		(L"may", boost::gregorian::May)
		(L"jun", boost::gregorian::Jun)
		(L"jul", boost::gregorian::Jul)
		(L"aug", boost::gregorian::Aug)
		(L"sep", boost::gregorian::Sep)
		(L"oct", boost::gregorian::Oct)
		(L"nov", boost::gregorian::Nov)
		(L"dec", boost::gregorian::Dec);

	month %= (
		qi::int_ |
		ParserCharTraits::no_case[monthNames]
	);
	#pragma endregion

	#pragma region Supported Date Formats
	dateYearMonthDay = (
		year >> '-' >> month >> '-' >> day
	)[qi::_val = phoenix::construct<Date>(qi::_1, qi::_2, qi::_3)];

	dateYearMonth = (
		year >> '-' >> month
	)[qi::_val = phoenix::construct<Date>(qi::_1, qi::_2, 1)];

	dateYear = (
		year
	)[qi::_val = phoenix::construct<Date>(qi::_1, boost::gregorian::Jan, 1)];

	dateMonthYear = (
		month >> year
	)[qi::_val = phoenix::construct<Date>(qi::_2, qi::_1, 1)];

	date %= (
		dateYearMonthDay |
		dateYearMonth |
		dateYear |
		dateMonthYear
	);
	#pragma endregion
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

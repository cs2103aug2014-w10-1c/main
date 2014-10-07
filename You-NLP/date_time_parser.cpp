//@author A0097630B
#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include <boost/fusion/sequence.hpp>
#include <windows.h>

#include "exceptions/parser_exception.h"
#include "date_time_parser.h"

namespace You {
namespace NLP {

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
			boost::posix_time::hours(0))
		]
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

	day %= (
		qi::int_
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

	dateDayMonth = (
		day >> month
	)[qi::_val = phoenix::bind(&constructDayMonthDate, qi::_1, qi::_2)];

	date %= (
		dateYearMonthDay |
		dateYearMonth |
		dateMonthYear |
		dateDayMonth |
		dateYear
	);
	#pragma endregion
}

DateTimeParser::Year DateTimeParser::parseFuzzyYear(
	const std::vector<ParserCharEncoding::char_type>& chars) {
	Year value = boost::lexical_cast<Year>(
		StringType(chars.begin(), chars.end()));
	if (chars.size() == 2) {
		return parseTwoDigitYear(value);
	} else {
		return value;
	}
}

DateTimeParser::Year DateTimeParser::parseTwoDigitYear(Year year) {
	assert(year >= 0);
	assert(year <= 99);

#ifdef _WIN32
	auto getTwoDigitYearMax = []() throw() {
		static const wchar_t* TWO_DIGIT_YEAR_MAX_KEY =
			L"Control Panel\\International\\Calendars\\TwoDigitYearMax";

		HKEY twoDigitYearMaxKey;
		if (RegOpenKeyEx(
				HKEY_CURRENT_USER,
				TWO_DIGIT_YEAR_MAX_KEY,
				0,
				KEY_READ,
				&twoDigitYearMaxKey) == ERROR_SUCCESS
		) {
			DWORD twoDigitYearMax = 0;
			DWORD size = sizeof(twoDigitYearMax);

			DWORD result = RegQueryValueEx(
				twoDigitYearMaxKey,
				L"1",
				nullptr,
				nullptr,
				reinterpret_cast<BYTE*>(&twoDigitYearMax),
				&size);

			RegCloseKey(twoDigitYearMaxKey);
			if (result == ERROR_SUCCESS) {
				return twoDigitYearMax;
			}
		}

		return static_cast<DWORD>(2030);
	};  // NOLINT(readability/braces)

	Year twoDigitYearMax = static_cast<Year>(getTwoDigitYearMax());

	Year twoDigitCentury = twoDigitYearMax / 100;
	Year twoDigitModulus = twoDigitYearMax % 100;

	Year resultCentury = twoDigitCentury - ((year > twoDigitModulus) ? 1 : 0);
	return resultCentury * 100 + year;
#else
	if (year >= 69) {
		return 1900 + year;
	} else {
		return 2000 + year;
	}
#endif
}

DateTimeParser::Date DateTimeParser::constructDayMonthDate(
	Day day, Month month) {
	ptime now = boost::posix_time::second_clock::local_time();
	Date today = now.date();

	Date result = Date(today.year(), month, day);
	if (result < today) {
		result = Date(today.year() + 1, result.month(), result.day());
	}

	return result;
}

}  // namespace NLP
}  // namespace You

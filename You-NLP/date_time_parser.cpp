//@author A0097630B
#include "stdafx.h"
#include <boost/lexical_cast.hpp>
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
	if (parse(string, result)) {
		return result;
	} else {
		throw ParserException();
	}
}

bool DateTimeParser::parse(const StringType& string, ptime& result) {
	return qi::phrase_parse(
		begin(string),
		end(string),
		DateTimeParser(),
		ParserSkipperType(),
		result);
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

	weekDays.add
		(L"mon", boost::gregorian::Monday)
		(L"monday", boost::gregorian::Monday)
		(L"tue", boost::gregorian::Tuesday)
		(L"tuesday", boost::gregorian::Tuesday)
		(L"wed", boost::gregorian::Wednesday)
		(L"wednesday", boost::gregorian::Wednesday)
		(L"thu", boost::gregorian::Thursday)
		(L"thursday", boost::gregorian::Thursday)
		(L"fri", boost::gregorian::Friday)
		(L"friday", boost::gregorian::Friday)
		(L"sat", boost::gregorian::Saturday)
		(L"saturday", boost::gregorian::Saturday)
		(L"sun", boost::gregorian::Sunday)
		(L"sunday", boost::gregorian::Sunday);

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
		dateYear |
		relativeDate
	);

	relativeDate %= (
		ParserCharTraits::no_case[(
			qi::lit(L"next") |
			qi::lit(L"coming"))] >>
		relativeDateInDirection(1) |

		ParserCharTraits::no_case[(
			qi::lit(L"this"))] >>
		relativeDateInDirection(0) |

		ParserCharTraits::no_case[(
			qi::lit(L"last") |
			qi::lit(L"previous"))] >>
		relativeDateInDirection(-1)
	);

	relativeDateInDirection = ParserCharTraits::no_case[(
		(
			monthNames
			[qi::_val = phoenix::bind(&
				constructRelativeMonthDate, qi::_r1, qi::_1)] |

			weekDays
			[qi::_val = phoenix::bind(&
				constructRelativeWeekDayDate, qi::_r1, qi::_1)]
		)
	)];
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

DateTimeParser::Date DateTimeParser::constructRelativeMonthDate(
	int direction,
	boost::date_time::months_of_year month) {
	boost::posix_time::ptime now =
		boost::posix_time::second_clock::local_time();
	Date today = now.date();

	assert(direction >= -1 && direction <= 1);
	if (direction == 0) {
		if (today.month() <= month) {
			throw ParserException();
		} else {
			return Date(today.year(), month, 1);
		}
	} else {
		return Date(today.year() + direction, month, 1);
	}
}

DateTimeParser::Date DateTimeParser::constructRelativeWeekDayDate(
	int direction,
	boost::date_time::weekdays day) {
	boost::posix_time::ptime now =
		boost::posix_time::second_clock::local_time();
	Date today = now.date();

	assert(direction >= -1 && direction <= 1);
	if (direction == 0) {
		if (today.day_of_week() <= day) {
			throw ParserException();
		} else {
			return today + boost::gregorian::days(day - today.day_of_week());
		}
	} else if (direction == 1) {
		return next_weekday(
			next_weekday(today, boost::gregorian::greg_weekday(
				boost::gregorian::Sunday)),
			boost::gregorian::greg_weekday(day));
	} else {
		return previous_weekday(
			previous_weekday(today, boost::gregorian::greg_weekday(
				boost::gregorian::Sunday)),
			boost::gregorian::greg_weekday(day));
	}
}

}  // namespace NLP
}  // namespace You

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
		boost::gregorian::months difference(
			month - today.month());

		// Because boost::date_time doesn't have operator <=
		if (difference.number_of_months() < 0 ||
			difference.number_of_months() == 0) {
			difference = difference + boost::gregorian::years(1);
		}
		Date result(today + boost::gregorian::months(difference));
		return Date(result.year(), result.month(), 1);
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
		boost::gregorian::days difference(
			static_cast<int>(day) - today.day_of_week());
		if (difference.days() <= 0) {
			difference = difference + boost::gregorian::weeks(1);
		}
		return today + difference;
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

DateTimeParser::Date DateTimeParser::constructRelativeDate(int daysFromToday) {
	return boost::posix_time::second_clock::local_time().date() +
		boost::gregorian::days(daysFromToday);
}

}  // namespace NLP
}  // namespace You

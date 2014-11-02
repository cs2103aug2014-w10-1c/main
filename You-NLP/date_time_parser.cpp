//@author A0097630B
#include "stdafx.h"
#include "exceptions/parse_error_exception.h"
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
	return qi::parse(
		begin(string),
		end(string),
		DateTimeParser() > qi::eoi,
		result);
}

DateTimeParser::DateTimeParser() : DateTimeParser::base_type(start) {
	start %= (
		date[qi::_val = phoenix::construct<DateTime>(
			qi::_1,
			boost::posix_time::hours(0))
		]
	);
	BOOST_SPIRIT_DEBUG_NODE(start);

	#pragma region Primitive date component parsers
	year = (
		+ParserCharTraits::digit
	)[qi::_val = phoenix::bind(&DateTimeParser::parseFuzzyYear, qi::_1)];
	BOOST_SPIRIT_DEBUG_NODE(year);

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
	monthNames.name("monthNames");

	month %= (
		qi::int_ |
		ParserCharTraits::no_case[monthNames]
	);
	month.name("month");

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
	weekDays.name("weekDays");

	day %= (
		qi::int_
	);
	BOOST_SPIRIT_DEBUG_NODE(day);
	#pragma endregion

	space = qi::omit[+ParserCharTraits::blank];

	#pragma region Supported Date Formats
	dateYearMonthDay = (
		year >> (space | '-') >> month >> (space | '-') >> day
	)[qi::_val = phoenix::construct<Date>(qi::_1, qi::_2, qi::_3)];
	BOOST_SPIRIT_DEBUG_NODE(dateYearMonthDay);

	dateYearMonth = (
		year >> '-' >> month
	)[qi::_val = phoenix::construct<Date>(qi::_1, qi::_2, 1)];
	BOOST_SPIRIT_DEBUG_NODE(dateYearMonth);

	dateYear = (
		year
	)[qi::_val = phoenix::construct<Date>(qi::_1, boost::gregorian::Jan, 1)];
	BOOST_SPIRIT_DEBUG_NODE(dateYear);

	dateMonthYear = (
		month >> space  >> year
	)[qi::_val = phoenix::construct<Date>(qi::_2, qi::_1, 1)];
	BOOST_SPIRIT_DEBUG_NODE(dateMonthYear);

	dateDayMonth = (
		day >> space >> month
	)[qi::_val = phoenix::bind(&constructDayMonthDate, qi::_1, qi::_2)];
	BOOST_SPIRIT_DEBUG_NODE(dateDayMonth);

	date %= (
		relativeDate |
		dateYearMonthDay |
		dateYearMonth |
		dateMonthYear |
		dateDayMonth |
		dateYear
	);
	BOOST_SPIRIT_DEBUG_NODE(date);
	#pragma endregion

	relativeDate %= (
		ParserCharTraits::no_case[(
			qi::lit(L"next") |
			qi::lit(L"coming"))] >> space >>
		relativeDateInDirection(1) |

		ParserCharTraits::no_case[(
			qi::lit(L"this"))] >> space >>
		relativeDateInDirection(0) |

		ParserCharTraits::no_case[(
			qi::lit(L"last") |
			qi::lit(L"previous"))] >> space >>
		relativeDateInDirection(-1) |

		relativeDateInDays
	);
	relativeDate.name("relativeDate");

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
	relativeDateInDirection.name("relativeDateInDirection");

	relativeDateInDays = (
		ParserCharTraits::no_case[qi::lit(L"tomorrow")][
			qi::_val = phoenix::bind(&constructRelativeDate, 1)
		] |

		ParserCharTraits::no_case[qi::lit(L"yesterday")][
			qi::_val = phoenix::bind(&constructRelativeDate, -1)
		] |

		((qi::int_ >> space >> ParserCharTraits::no_case[qi::lit("days")])[
			qi::_val = phoenix::bind(&constructRelativeDate, qi::_1)
		]) |

		((qi::int_ >> space >> ParserCharTraits::no_case[qi::lit("weeks")])[
			qi::_val = phoenix::bind(&constructRelativeDate,
				qi::_1 * 7)
		])
	);
	relativeDateInDays.name("relativeDateInDays");
	#pragma endregion

	qi::on_error<qi::fail>(start,
		phoenix::bind(&onFailure, qi::_1, qi::_2, qi::_3, qi::_4));
}

void DateTimeParser::onFailure(
	ParserIteratorType /*begin*/,
	ParserIteratorType end,
	ParserIteratorType errorPos,
	const boost::spirit::info& message) {
	StringType lexeme(errorPos, end);
	throw ParseErrorException(message, lexeme);
}

}  // namespace NLP
}  // namespace You

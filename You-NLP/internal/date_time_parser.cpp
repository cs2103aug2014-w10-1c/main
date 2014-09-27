#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include <boost/fusion/sequence.hpp>
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
	start %= (
		date[qi::_val = phoenix::construct<ptime>(qi::_1, boost::posix_time::hours(0))]
	);

	date = (
		year |
		(year >> month)
	)[qi::_val = phoenix::bind(&DateTimeParser::parsePartialDate, qi::_1)];

	year = (
		+ParserCharTraits::digit
	)[qi::_val = phoenix::bind(&DateTimeParser::parseFuzzyYear, qi::_1)];

	qi::symbols<
		ParserCharEncoding::char_type,
		boost::gregorian::months_of_year> sym;

	month %= (
		qi::int_ |
		ParserCharTraits::no_case[sym]
	);
}

date DateTimeParser::parsePartialDate(
	const boost::variant<
		int16_t,
		boost::fusion::vector<int16_t, boost::gregorian::months_of_year>
	> & dateParts) {
	using date = Internal::date;
	class PartialDateVisitor : public boost::static_visitor<date> {
	public:
		date operator()(const int16_t& year) {
			return date(year, boost::gregorian::NotAMonth, 1);
		}

		date operator()(const boost::fusion::vector<
			int16_t, boost::gregorian::months_of_year> & parts) {
			return date(
				boost::fusion::at_c<0>(parts),
				boost::fusion::at_c<1>(parts),
				1);
		}
	};

	PartialDateVisitor visitor;
	return boost::apply_visitor(visitor, dateParts);
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

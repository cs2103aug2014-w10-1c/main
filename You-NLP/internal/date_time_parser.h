#pragma once
#ifndef YOU_NLP_INTERNAL_DATE_TIME_PARSER
#define YOU_NLP_INTERNAL_DATE_TIME_PARSER

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include "query_parser.h"

namespace You {
namespace NLP {
namespace Internal {

using date = boost::gregorian::date;

/// The query parser that recognises our input syntax.
class DateTimeParser {
public:
	/// Parses the given input string into date.
	///
	/// \param[in] string The string to parse.
	/// \return The date representing the string.
	static date parse(const std::wstring& string);
};

}
}
}
#endif

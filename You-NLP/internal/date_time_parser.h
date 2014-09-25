#pragma once
#ifndef YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_
#define YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include "query_parser.h"

namespace You {
namespace NLP {
namespace Internal {

using date = boost::gregorian::date;
using ptime = boost::posix_time::ptime;

/// The query parser that recognises our input syntax.
class DateTimeParser {
public:
	/// Parses the given input string into date.
	///
	/// \param[in] string The string to parse.
	/// \return The date/time representing the string.
	static ptime parse(const std::wstring& string);
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_

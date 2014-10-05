#pragma once
#ifndef YOU_NLP_PARSER_H_
#define YOU_NLP_PARSER_H_

#include <boost/spirit/include/qi.hpp>

namespace You {
namespace NLP {

/// Import the entire character traits namespace from Spirit.
namespace ParserCharTraits {
	using namespace boost::spirit::qi::standard_wide; // NOLINT
}

/// The type of one character in the lexer stream.
typedef boost::spirit::char_encoding::standard_wide ParserCharEncoding;

/// The type of the parser iterator.
typedef std::wstring::const_iterator ParserIteratorType;

/// The type of the parser skipper.
typedef ParserCharTraits::space_type ParserSkipperType;

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSER_H_

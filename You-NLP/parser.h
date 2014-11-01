//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSER_H_
#define YOU_NLP_PARSER_H_

#include <boost/spirit/include/qi.hpp>
#include "You-Utils/log.h"

namespace You {
namespace NLP {

/// Import the entire character traits namespace from Spirit.
namespace ParserCharTraits {
	// NOLINTNEXTLINE(build/namespaces)
	using namespace boost::spirit::qi::standard_wide;
}

/// The type of one character in the lexer stream.
typedef boost::spirit::char_encoding::standard_wide ParserCharEncoding;

/// The type of the parser iterator.
typedef std::wstring::const_iterator ParserIteratorType;

/// The type of the parser skipper.
typedef ParserCharTraits::space_type ParserSkipperType;

}  // namespace NLP
}  // namespace You

namespace boost {
namespace spirit {
namespace qi {
namespace detail {

template<typename Char>
inline void token_printer(You::Utils::Logger& m, Char c) {
	// allow to customize the token printer routine
	m << c;
}

}
}
}
}

#endif  // YOU_NLP_PARSER_H_

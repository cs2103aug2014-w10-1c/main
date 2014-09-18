#pragma once
#ifndef YOU_NLP_INTERNAL_PARSER_H_
#define YOU_NLP_INTERNAL_PARSER_H_

#include <boost/spirit/include/qi.hpp>

#include "parse_tree.h"

namespace You {
namespace NLP {
namespace Internal {

namespace qi = boost::spirit::qi;
typedef std::wstring::const_iterator ParserIteratorType;
typedef boost::spirit::standard_wide::space_type ParserSkipperType;

class Parser : public qi::grammar<
	ParserIteratorType,
	Query(),
	ParserSkipperType> {

	/// The type of the iterator used in this grammar.
	typedef ParserIteratorType IteratorType;

	/// The type of the skipper used in this grammar.
	typedef ParserSkipperType SkipperType;
	
public:
	Parser();

private:
	/// The start rule.
	start_type start;

	/// Explicit command rule.
	qi::rule<IteratorType, Query(), SkipperType> explicitCommand;

	/// Add command rule.
	qi::rule<IteratorType, AddQuery(), SkipperType> addCommand;
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_PARSER_H_

//@author A0097630B
#pragma once
#ifndef YOU_NLP_EXCEPTIONS_PARSE_ERROR_EXCEPTION_H_
#define YOU_NLP_EXCEPTIONS_PARSE_ERROR_EXCEPTION_H_

#include "parser_exception.h"
#include "../query_parser.h"

namespace You {
namespace NLP {

/// Represents a parse error in the NLP engine.
class ParseErrorException : public ParserException {
public:
	/// Constructor.
	///
	/// \param[in] message The Spirit parse error information.
	/// \param[in] token The token being parsed.
	ParseErrorException(
		boost::spirit::info message,
		QueryParser::StringType token);

private:
	/// The Spirit parse error.
	boost::spirit::info message;

	/// The token causing the error.
	QueryParser::StringType token;
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_EXCEPTIONS_PARSE_ERROR_EXCEPTION_H_

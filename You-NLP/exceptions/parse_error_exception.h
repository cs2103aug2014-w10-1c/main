#pragma once
#ifndef YOU_NLP_EXCEPTIONS_PARSE_ERROR_EXCEPTION_H_
#define YOU_NLP_EXCEPTIONS_PARSE_ERROR_EXCEPTION_H_

#include "parser_exception.h"
#include "../internal/parser.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;
using Parser = Internal::Parser;

/// Represents a parse error in the NLP engine.
class ParseErrorException : public ParserException {
public:
	/// Constructor.
	///
	/// \param[in] message The Spirit parse error information.
	/// \param[in] token The token being parsed.
	ParseErrorException(spirit::info message, Parser::StringType token);

private:
	/// The Spirit parse error.
	spirit::info message;

	/// The token causing the error.
	Parser::StringType token;
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_EXCEPTIONS_PARSE_ERROR_EXCEPTION_H_

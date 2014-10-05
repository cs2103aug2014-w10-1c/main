#include "stdafx.h"
#include "parse_error_exception.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;

ParseErrorException::ParseErrorException(
	spirit::info message,
	QueryParser::StringType token)
: message(message),
  token(token) {
}

}  // namespace NLP
}  // namespace You

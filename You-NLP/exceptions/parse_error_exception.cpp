#include "stdafx.h"
#include "parse_error_exception.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;
using Parser = Internal::Parser;

ParseErrorException::ParseErrorException(
	spirit::info message,
	Parser::StringType token)
: message(message),
  token(token) {

}


}  // namespace NLP
}  // namespace You

//@author A0097630B
#include "stdafx.h"
#include "parse_error_exception.h"

namespace You {
namespace NLP {

namespace spirit = boost::spirit;

ParseErrorException::ParseErrorException(
	spirit::info message,
	std::wstring token)
: message(message),
  token(token) {
}

std::wstring ParseErrorException::getToken() {
	return token;
}
boost::spirit::info ParseErrorException::getMessage() {
	return message;
}
}  // namespace NLP
}  // namespace You

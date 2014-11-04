#include "stdafx.h"
#include "xml_parse_error_exception.h"

namespace You {
namespace DataStore {

XmlParseErrorException::XmlParseErrorException(std::string errorMessage)
: errorMessage(errorMessage) {
}

std::string XmlParseErrorException::getErrorMessage() {
	return errorMessage;
}

}  // namespace DataStore
}  // namespace You

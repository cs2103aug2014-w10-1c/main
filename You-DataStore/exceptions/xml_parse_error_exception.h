//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_EXCEPTIONS_XML_PARSE_ERROR_EXCEPTION_H_
#define YOU_DATASTORE_EXCEPTIONS_XML_PARSE_ERROR_EXCEPTION_H_

#include "exception.h"

namespace You {
namespace DataStore {

/// The exception thrown if there is an error when parsing data.xml
class XmlParseErrorException : public DataStoreException {
public:
	/// Constructor
	///
	/// \param[in] errorMessage The error message
	explicit XmlParseErrorException(std::string errorMessage);

	/// Get the error message
	///
	/// \return The error message
	std::string getErrorMessage();

private:
	std::string errorMessage;
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_EXCEPTIONS_XML_PARSE_ERROR_EXCEPTION_H_

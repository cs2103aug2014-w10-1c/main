//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_EXCEPTIONS_DATASTORE_CORRUPT_EXCEPTION_H_
#define YOU_DATASTORE_EXCEPTIONS_DATASTORE_CORRUPT_EXCEPTION_H_

#include "exception.h"

namespace You {
namespace DataStore {

/// The exception thrown if there is an error when parsing data.xml
class DataStoreCorruptException : public DataStoreException {
};

/// The exception thrown if data.xml is not well-formed XML
class NotWellFormedXmlException : public DataStoreCorruptException {
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_EXCEPTIONS_DATASTORE_CORRUPT_EXCEPTION_H_

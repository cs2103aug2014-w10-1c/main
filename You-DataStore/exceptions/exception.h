//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_EXCEPTIONS_EXCEPTION_H_
#define YOU_DATASTORE_EXCEPTIONS_EXCEPTION_H_

#include "../../You-Utils/exceptions/exception.h"

namespace You {
namespace DataStore {

/// Base exception class for Data Store exceptions
class DataStoreException : public You::Utils::Exception {
};

/// The exception thrown if there is an I/O error when parsing data.xml
class IOException : public DataStoreException {
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_EXCEPTIONS_EXCEPTION_H_

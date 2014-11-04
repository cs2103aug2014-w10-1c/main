#pragma once
#ifndef YOU_DATASTORE_EXCEPTIONS_EXCEPTION_H_
#define YOU_DATASTORE_EXCEPTIONS_EXCEPTION_H_

#include "../../You-Utils/exceptions/exception.h"

namespace You {
namespace DataStore {
namespace Exception {

/// Base exception class for Data Store exceptions
class DataStoreException : public You::Utils::Exception {
};

}  // namespace Exception
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_EXCEPTIONS_EXCEPTION_H_

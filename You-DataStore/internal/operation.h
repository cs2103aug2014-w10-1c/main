#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATION_H_

#include <unordered_map>
#include "internal_datastore.h"

namespace You {
namespace DataStore {

/// A pure virtual class of operations to be put into transaction stack
class IOperation {
public:
	IOperation();
	virtual ~IOperation();

	/// Executes the operation
	virtual void run(InternalDataStore::STask&) = 0;
};

}   // namespace DataStore
}   // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATION_H_

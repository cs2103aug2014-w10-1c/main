#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATION_H_

#include <unordered_map>
#include "../ds_task_typedefs.h"

namespace You {
namespace DataStore {

/// A pure virtual class of operations to be put into transaction stack
class IOperation {
public:
	/// The constructor
	/// \param stask the serialized task the operation need to work with
	explicit IOperation(SerializedTask& stask);
	virtual ~IOperation();

	/// Executes the operation
	virtual void run() = 0;
};

}   // namespace DataStore
}   // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATION_H_

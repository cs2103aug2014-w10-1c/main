#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {
class PutOperation : public IOperation {
public:
	PutOperation(TaskId, const SerializedTask&);
	bool run();
};
}  // namespace Internal
}   // namespace DataStore
}   // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_ERASE_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_ERASE_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {
class EraseOperation : public IOperation {
public:
	explicit EraseOperation(TaskId);
	bool run();
};
}  // namespace Internal
}   // namespace DataStore
}   // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_ERASE_OPERATION_H_

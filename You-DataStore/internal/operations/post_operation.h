#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {
class PostOperation : public IOperation {
public:
	PostOperation(TaskId, SerializedTask&);
	bool run();
};
}  // namespace Internal
}   // namespace DataStore
}   // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_

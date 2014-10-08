#include "stdafx.h"
#include "../../datastore.h"
#include "erase_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

EraseOperation::EraseOperation(TaskId id) {
	taskId = id;
}

bool EraseOperation::run() {
	return DataStore::get().getInternal().erase(taskId);
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

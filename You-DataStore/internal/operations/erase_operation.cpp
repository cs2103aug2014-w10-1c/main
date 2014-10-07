#include "stdafx.h"
#include "erase_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

EraseOperation::EraseOperation(TaskId id) {
	taskId = id;
}

bool EraseOperation::run() {
	InternalDataStore ids;
	return ids.erase(taskId);
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

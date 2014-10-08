#include "stdafx.h"
#include "../../task_typedefs.h"
#include "../internal_datastore.h"
#include "erase_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

EraseOperation::EraseOperation(TaskId id) {
	taskId = id;
}

bool EraseOperation::run() {
	return false;
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

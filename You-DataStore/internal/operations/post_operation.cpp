#include "stdafx.h"
#include "../../task_typedefs.h"
#include "../internal_datastore.h"
#include "post_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PostOperation::PostOperation(TaskId id, const SerializedTask& stask) {
	taskId = id;
	task = stask;
}

bool PostOperation::run() {
	return false;
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

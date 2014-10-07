#include "stdafx.h"
#include "post_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PostOperation::PostOperation(TaskId id, const SerializedTask& stask) {
	taskId = id;
	task = stask;
}

bool PostOperation::run() {
	InternalDataStore ids;
	return ids.post(taskId, task);
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

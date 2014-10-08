#include "stdafx.h"
#include "../../task_typedefs.h"
#include "../internal_datastore.h"
#include "put_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PutOperation::PutOperation(TaskId id, const SerializedTask& stask) {
	taskId = id;
	task = stask;
}

bool PutOperation::run() {
	return false;
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

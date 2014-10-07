#include "stdafx.h"
#include "post_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

bool PostOperation::run() {
	InternalDataStore ids;
	return ids.post(taskId, task);
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

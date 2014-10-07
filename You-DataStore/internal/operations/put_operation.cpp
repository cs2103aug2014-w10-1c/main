#include "stdafx.h"
#include "put_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

bool PutOperation::run() {
	InternalDataStore ids;
	return ids.put(taskId, task);
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

/// \author A0112054Y
#include "stdafx.h"

#include "../state.h"
#include "delete_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

Response DeleteTask::execute(State& tasks) {
	tasks.graph().deleteTask(this->id);
	return this->id;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

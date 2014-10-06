/// \author A0112054Y
#include "stdafx.h"

#include "delete_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

Response DeleteTask::execute(State& tasks) {
#if 0
	queryEngine.getTaskGraph().remove(this->id)

	Transaction t(dataStorage.begin());
	dataStorage.remove(this->id);
	t.commit();
#endif
	return this->id;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

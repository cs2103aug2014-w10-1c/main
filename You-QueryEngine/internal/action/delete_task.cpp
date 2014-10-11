/// \author A0112054Y
#include "stdafx.h"

#include "../state.h"
#include "../controller.h"
#include "delete_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

void DeleteTask::makeTransaction() {
	Transaction t(DataStore::get().begin());
	DataStore::get().erase(id);
	t.commit();
}

Response DeleteTask::execute(State& state) {
	Controller::Graph::deleteTask(state.graph(), this->id);
	makeTransaction();
	return this->id;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

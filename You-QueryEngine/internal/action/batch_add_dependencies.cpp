/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../controller.h"
#include "add_task.h"
#include "batch_add_dependencies.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// \cond Imports
namespace {
	using Transaction = You::DataStore::Transaction;
	using DataStore = You::DataStore::DataStore;
	using Log = You::Utils::Log;
}
/// \endcond

const std::wstring BatchAddDependencies::logCategory =
	Query::logCategory + L"[BatchAddDependencies]";

Response BatchAddDependencies::execute(State& state) {
	auto qBegin = dependencies.begin();
	auto qEnd = dependencies.end();
	Task::ID lastInserted =
		boost::get<Task>((*qBegin)->execute(state)).getID();
	++qBegin;
	for (qBegin; qBegin != qEnd; ++qBegin) {
		Response r = (*qBegin)->execute(state);
		auto task = boost::get<Task>(r);
		task.setDependencies({ lastInserted });
		Controller::Graph::updateTask(state.get().graph(),
			task);
		Controller::Graph::updateTask(state.get().sgraph(),
			task);
		lastInserted = task.getID();
	}

	auto addParentquery = (insertedID == -1)
		? std::unique_ptr<AddTask>(new AddTask(description,
		deadline, priority, { lastInserted }, subtasks))
		: std::unique_ptr<AddTask>(new AddTask(insertedID, description,
		deadline, priority, { lastInserted }, subtasks));

	Response r = addParentquery->execute(state);

	Task newTask = boost::get<Task>(r);
	insertedID = newTask.getID();

	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

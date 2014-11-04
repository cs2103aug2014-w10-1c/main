//@author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"

#include "../controller.h"
#include "update_task.h"
#include "add_task.h"
#include "batch_add_dependencies.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// \cond Imports
namespace {
	using Log = You::Utils::Log;
}
/// \endcond

const std::wstring BatchAddDependencies::logCategory =
	Query::logCategory + L"[BatchAddDependencies]";

std::unique_ptr<Query> BatchAddDependencies::getReverse() {
	return QueryEngine::DeleteTask(insertedID);
}

Response BatchAddDependencies::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : BEGIN") % logCategory).str();
	auto qBegin = dependencies.begin();
	auto qEnd = dependencies.end();
	Task::ID lastInserted =
		boost::get<Task>((*qBegin)->execute(state)).getID();
	++qBegin;

	for (qBegin; qBegin != qEnd; ++qBegin) {
		Response r = (*qBegin)->execute(state);
		auto task = boost::get<Task>(r);
		task.setDependencies({ lastInserted });
		UpdateTask(task).execute(state);
		lastInserted = task.getID();
	}

	std::unique_ptr<Query> addParentQuery;
	if (insertedID == -1) {
		addParentQuery = std::unique_ptr<AddTask>(
			new AddTask(description, deadline, priority,
			{ lastInserted }, subtasks));
	} else {
		addParentQuery = std::unique_ptr<AddTask>(
			new AddTask(insertedID, description,
			deadline, priority,
			{ lastInserted }, subtasks));
	}
	insertedID = lastInserted;
	Response r = addParentQuery->execute(state);
	Log::debug << (boost::wformat(L"%1% : END") % logCategory).str();
	return boost::get<Task>(r);
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

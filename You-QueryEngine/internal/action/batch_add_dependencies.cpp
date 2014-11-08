//@author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"

#include "add_task.h"
#include "batch_add_dependencies.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

using Log = You::Utils::Log;

const std::wstring BatchAddDependencies::logCategory =
	Query::logCategory + L"[BatchAddDependencies]";

std::unique_ptr<Query> BatchAddDependencies::getReverse() {
	return QueryEngine::DeleteTask(insertedID);
}

Task::ID BatchAddDependencies::executeDependenciesAddQuery(State& state) {
	auto qBegin = dependencies.begin();
	auto qEnd = dependencies.end();
	Task::ID lastInserted =
		boost::get<Task>((*qBegin)->execute(state)).getID();
	++qBegin;

	for (qBegin; qBegin != qEnd; ++qBegin) {
		Response r = (*qBegin)->execute(state);
		auto task = boost::get<Task>(r);
		task.setDependencies({ lastInserted });
		QueryEngine::UpdateTask(task)->execute(state);
		lastInserted = task.getID();
	}
	return lastInserted;
}

Task BatchAddDependencies::executeParentAddQuery(
	State& state, const Task::ID lastInserted) {
	std::unique_ptr<Query> addParentQuery;
	if (insertedID == -1) {
		addParentQuery = std::unique_ptr<AddTask>(
			new AddTask(description, startTime,
				deadline, priority, { lastInserted },
				subtasks));
	} else {
		addParentQuery = std::unique_ptr<AddTask>(
			new AddTask(insertedID, description,
				startTime, deadline, priority,
				{ lastInserted }, subtasks));
	}
	Task parent = boost::get<Task>(addParentQuery->execute(state));
	insertedID = parent.getID();
	return parent;
}

Response BatchAddDependencies::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : BEGIN") % logCategory).str();
	Task::ID lastInserted = executeDependenciesAddQuery(state);
	Task parent = executeParentAddQuery(state, lastInserted);
	Log::debug << (boost::wformat(L"%1% : END") % logCategory).str();
	return parent;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

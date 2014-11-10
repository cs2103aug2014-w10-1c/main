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
	class BatchDeleteDependencies : public Query {
	public:
		explicit BatchDeleteDependencies(
			const std::vector<Task::ID>& dependencies)
		: dependencies(dependencies) {}

		Response execute(State& state) {
			for (const auto& id : dependencies) {
				QueryEngine::DeleteTask(id)->execute(state);
			}
			return dependencies.back();
		}
	private:
		const std::vector<Task::ID> dependencies;
		BatchDeleteDependencies operator=(
			const BatchDeleteDependencies&) = delete;
	};
	return std::unique_ptr<Query>(new BatchDeleteDependencies(insertedIDs));
}

Task::ID BatchAddDependencies::executeDependenciesAddQuery(State& state) {
	auto qBegin = dependencies.begin();
	auto qEnd = dependencies.end();
	Task::ID lastInserted =
		boost::get<Task>((*qBegin)->execute(state)).getID();
	++qBegin;

	insertedIDs.emplace_back(lastInserted);
	for (qBegin; qBegin != qEnd; ++qBegin) {
		Response r = (*qBegin)->execute(state);
		auto task = boost::get<Task>(r);
		task.setDependencies({ lastInserted });
		QueryEngine::UpdateTask(task)->execute(state);
		lastInserted = task.getID();
		insertedIDs.emplace_back(lastInserted);
	}

	return lastInserted;
}

Task BatchAddDependencies::executeParentAddQuery(State& state) {
	std::unique_ptr<Query> addParentQuery;
	Task::ID lastInserted = insertedIDs.back();
	Response response =
		AddTask(description, startTime, deadline, priority,
			{ lastInserted }, subtasks).execute(state);
	Task parent = boost::get<Task>(response);
	insertedIDs.emplace_back(parent.getID());
	return parent;
}

Response BatchAddDependencies::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : BEGIN") % logCategory).str();
	executeDependenciesAddQuery(state);
	Task parent = executeParentAddQuery(state);
	Log::debug << (boost::wformat(L"%1% : END") % logCategory).str();
	return parent;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

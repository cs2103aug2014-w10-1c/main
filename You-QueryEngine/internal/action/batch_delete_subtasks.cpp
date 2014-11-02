/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "add_task.h"
#include "delete_task.h"
#include "batch_add_subtasks.h"
#include "batch_delete_subtasks.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

namespace {
	using Transaction = You::DataStore::Transaction;
	using DataStore = You::DataStore::DataStore;
	using Log = You::Utils::Log;
}

const std::wstring BatchDeleteSubTasks::logCategory =
	Query::logCategory + L"[BatchDeleteSubTasks]";

std::unique_ptr<Query> BatchDeleteSubTasks::getReverse() {
	std::vector<std::unique_ptr<Query>> subtaskAddQueries;
	// Construct add query for each subtask
	for (const auto& cid : deletedParent.getSubtasks()) {
		auto c = childrens[cid];
		subtaskAddQueries.push_back(
			std::unique_ptr<Query>(
				new AddTask(
					c.getDescription(),
					c.getDeadline(),
					c.getPriority(),
					c.getDependencies(),
					c.getSubtasks())));
	}

	return std::unique_ptr<Query>(new BatchAddSubTasks(
		deletedParent.getID(),
		deletedParent.getDescription(),
		deletedParent.getDeadline(),
		deletedParent.getPriority(),
		deletedParent.getDependencies(),
		std::move(subtaskAddQueries)));
}

void BatchDeleteSubTasks::constructUndoTree(
	State& state, Task::ID id) {
	Task c = state.get().graph().getTask(id);
	for (auto cid : c.getSubtasks()) {
		constructUndoTree(state, cid);
	}
	childrens.insert({ id, c });
}

void BatchDeleteSubTasks::deleteTree(State& state, Task::ID id) {
	Task c = state.get().graph().getTask(id);
	for (auto cid : c.getSubtasks()) {
		deleteTree(state, cid);
	}
	DeleteTask(id).execute(state);
}

Response BatchDeleteSubTasks::execute(State& state) {
	deletedParent = state.get().graph().getTask(id);
	deleteTree(state, id);
	return this->id;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

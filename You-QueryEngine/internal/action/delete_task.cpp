/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "add_task.h"
#include "delete_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

namespace {
	using Transaction = You::DataStore::Transaction;
	using DataStore = You::DataStore::DataStore;
	using Log = You::Utils::Log;
}

const std::wstring DeleteTask::logCategory =
	Query::logCategory + L"[DeleteTask]";

std::unique_ptr<Query> DeleteTask::getReverse() {
	return std::unique_ptr<Query>(new AddTask(
		deletedTask.getDescription(), deletedTask.getDeadline(),
		deletedTask.getPriority(), deletedTask.getDependencies()));
}

void DeleteTask::makeTransaction() {
	Log::debug << (boost::wformat(L"%1% : ERASE \"%2%\"\n")
		% logCategory % id).str();
	Transaction t(DataStore::get().begin());
	DataStore::get().erase(id);
	t.commit();
}

Response DeleteTask::execute(State& state) {
	deletedTask = state.get().graph().getTask(id);
	Controller::Graph::deleteTask(state.graph(), this->id);
	Controller::Graph::deleteTask(state.sgraph(), this->id);
	makeTransaction();
	return this->id;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

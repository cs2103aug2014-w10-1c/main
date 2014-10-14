/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
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

const std::wstring DeleteTask::category = Query::category + L"[DeleteTask]";

void DeleteTask::makeTransaction() {
	Log::debug << (boost::wformat(L"%1% : ERASE \"%2%\"") %
		category % id).str().c_str();
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

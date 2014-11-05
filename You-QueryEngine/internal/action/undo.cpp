//@author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "get_task.h"
#include "undo.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

namespace {
	using Transaction = You::DataStore::Transaction;
	using DataStore = You::DataStore::DataStore;
	using Log = You::Utils::Log;
}

Response Undo::execute(State& state) {
	if (!State::get().undoStack().empty()) {
		std::unique_ptr<Query> query = std::move(state.get().undoStack().top());
		state.get().undoStack().pop();
		query->execute(state);
		std::unique_ptr<Query> refresh = std::unique_ptr<Query>(new GetTask(
			state.getActiveFilter(), state.getActiveComparator()));
		return refresh->execute(state);
	} else {
		throw Exception::NotUndoAbleException();
	}
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

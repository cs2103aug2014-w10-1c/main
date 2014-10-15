/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "update_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

namespace {
	using Transaction = You::DataStore::Transaction;
	using DataStore = You::DataStore::DataStore;
	using Log = You::Utils::Log;
}

const std::wstring UpdateTask::logCategory =
	Query::logCategory + L"[UpdateTask]";

Task UpdateTask::buildUpdatedTask(const State& state) const {
	auto current = state.get().graph().getTask(this->id);
	auto builder = Controller::Builder::fromTask(current);
	if (this->description == Task::DEFAULT_DESCRIPTION) {
		builder.description(current.getDescription());
	} else {
		builder.description(this->description);
	}
	if (this->deadline != Task::DEFAULT_DEADLINE) {
		builder.deadline(this->deadline);
	}
	if (this->priority != Task::DEFAULT_PRIORITY) {
		builder.priority(this->priority);
	}
	if (this->dependencies != Task::DEFAULT_DEPENDENCIES) {
		builder.dependencies(this->dependencies);
	}
	Task newTask = builder;
	newTask.setCompleted(this->completed);
	return newTask;
}

void UpdateTask::modifyState(State& state, const Task& task) const {
	Log::debug << [=] { return boost::wformat(L"%1% : Deleting %2% - \"%3%\"") %
		logCategory % task.getID() % task.getDescription(); };
	Controller::Graph::updateTask(state.graph(), task);
}

void UpdateTask::makeTransaction(const Task& updated) const {
	auto serialized = Controller::Serializer::serialize(updated);
	Transaction t(DataStore::get().begin());
	DataStore::get().put(this->id, serialized);
	t.commit();
}

Response UpdateTask::execute(State& state) {
	auto updated = buildUpdatedTask(state);
	modifyState(state, updated);
	return updated;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

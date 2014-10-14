/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "add_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// \cond Imports
namespace {
	using Transaction = You::DataStore::Transaction;
	using DataStore = You::DataStore::DataStore;
}
/// \endcond

Task AddTask::buildTask(const Task::ID newID) {
	return Controller::Builder::get().id(newID)
		.description(this->description)
		.deadline(this->deadline)
		.dependencies(this->dependencies)
		.priority(this->priority);
}

void AddTask::addTaskToState(const Task& task,
	State& state) const {
	Controller::Graph::addTask(state.graph(), task);
}

void AddTask::makeTransaction(const Task& newTask) const {
	auto serialized = Controller::Serializer::serialize(newTask);
	Transaction t(DataStore::get().begin());
	DataStore::get().post(newTask.getID(), serialized);
	t.commit();
}

Response AddTask::execute(State& state) {
	auto newTask = buildTask(state.inquireNewID());
	addTaskToState(newTask, state);
	makeTransaction(newTask);
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

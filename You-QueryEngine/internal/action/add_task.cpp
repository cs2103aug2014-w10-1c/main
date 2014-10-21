/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "delete_task.h"
#include "add_task.h"

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

const std::wstring AddTask::logCategory = Query::logCategory + L"[AddTask]";

std::unique_ptr<Query> AddTask::getReverse() {
	return std::unique_ptr<Query>(new DeleteTask(insertedID));
}

Task AddTask::buildTask(const Task::ID newID) {
	return Controller::Builder::get().id(newID)
		.description(this->description)
		.deadline(this->deadline)
		.dependencies(this->dependencies)
		.priority(this->priority);
}

void AddTask::ensureDependencyIsValid() const {
	std::for_each(dependencies.cbegin(), dependencies.cend(),
		[] (const Task::ID& id) {
			if (!Controller::Graph::isTaskExist(
					State::get().graph(), id)) {
				throw Exception::TaskNotFoundException();
			}
		}
	);
}

void AddTask::addTaskToState(const Task& task,
	State& state) const {
	Log::debug << (boost::wformat(L"%1% : Registering \"%2%\"\n") %
		logCategory % task.getDescription()).str();
	Controller::Graph::addTask(state.graph(), task);
}

void AddTask::makeTransaction(const Task& newTask) const {
	auto serialized = Controller::Serializer::serialize(newTask);
	Transaction t(DataStore::get().begin());
	Log::debug << (boost::wformat(L"%1% : POST \"%2%\"\n") %
		logCategory % newTask.getID()).str();
	DataStore::get().post(newTask.getID(), serialized);
	t.commit();
}

Response AddTask::execute(State& state) {
	auto newId = state.inquireNewID();
	auto newTask = buildTask(state.inquireNewID());
	ensureDependencyIsValid();
	addTaskToState(newTask, state);
	makeTransaction(newTask);
	insertedID = newId;
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

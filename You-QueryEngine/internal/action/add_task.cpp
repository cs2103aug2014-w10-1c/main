/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../model.h"
#include "../controller.h"
#include "update_task.h"
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
		.priority(this->priority)
		.dependencies(this->dependencies)
		.subtasks(this->subtasks);
}

void AddTask::ensureDependencyIsValid() const {
	for (const auto& id : dependencies) {
		if (!Controller::Graph::isTaskExist(
			State::get().graph(), id)) {
			throw Exception::TaskNotFoundException();
		}
	}
}

void AddTask::ensureSubtasksIsValid() const {
	for (const auto& id : subtasks) {
		try {
			auto task = State::get().sgraph().getTask(id);
			if (!task.isTopLevel()) {
				throw Exception::TaskAlreadyHasParentException();
			}
		} catch (const Exception::TaskNotFoundException& e) {
			throw e;
		} catch (const Exception::TaskAlreadyHasParentException& e) {
			throw e;
		}
	}
}

void AddTask::updateParentPointer(State& state) const {
	for (const auto& id : subtasks) {
		auto previous = State::get().sgraph().getTask(id);
		previous.setParent(insertedID);
		UpdateTask(previous).execute(state);
	}
}

void AddTask::addTaskToGraphs(const Task& task,
	State& state) const {
	Log::debug << (boost::wformat(L"%1% : Registering \"%2%\"\n") %
		logCategory % task.getDescription()).str();
	Controller::Graph::addTask(state.graph(), task);
	Controller::Graph::addTask(state.sgraph(), task);
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
	bool isFirstTime = false;
	if (insertedID == -1) {
		insertedID = state.inquireNewID();
	} else if (insertedID == 0) {
		isFirstTime = true;
	}
	auto newTask = buildTask(insertedID);
	ensureSubtasksIsValid();
	ensureDependencyIsValid();
	addTaskToGraphs(newTask, state);
	makeTransaction(newTask);
	updateParentPointer(state);

	state.commitMaxIDToDataStore();
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

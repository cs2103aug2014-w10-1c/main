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
			if (task.getParent() != task.getID()) {
				throw Exception::TaskAlreadyHasParentException();
			}
		} catch (const Exception::TaskNotFoundException& e) {
			throw e;
		} catch (const Exception::TaskAlreadyHasParentException& e) {
			throw e;
		}
	}
}

void AddTask::updateParentPointer() const {
	for (const auto& id : subtasks) {
		auto previous = State::get().sgraph().getTask(id);
		Controller::Graph::updateTask(
			State::get().sgraph(),
			Controller::Builder::fromTask(previous)
				.parent(this->insertedID));
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
	auto newId = state.inquireNewID();
	auto newTask = buildTask(newId);
	ensureSubtasksIsValid();
	ensureDependencyIsValid();
	addTaskToGraphs(newTask, state);
	makeTransaction(newTask);
	insertedID = newId;
	updateParentPointer();
	state.commitMaxIDToDataStore(false);
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

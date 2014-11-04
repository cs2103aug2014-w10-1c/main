//@author A0112054Y
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

std::unique_ptr<Query> UpdateTask::getReverse() {
	return std::unique_ptr<Query>(new UpdateTask(
		previous.getID(), previous.getDescription(),
		previous.getDeadline(), previous.getPriority(),
		previous.getDependencies(), previous.isCompleted(),
		previous.getParent(), previous.getSubtasks()));
}

Task UpdateTask::buildUpdatedTask(const State& state) const {
	auto current = state.get().graph().getTask(this->id);
	auto builder = Controller::Builder::fromTask(current);

	#pragma region Update the fields iff it is requested
	if (this->description) {
		builder.description(this->description.get());
	}
	if (this->deadline) {
		builder.deadline(this->deadline.get());
	}
	if (this->priority) {
		builder.priority(this->priority.get());
	}
	if (this->dependencies) {
		builder.dependencies(this->dependencies.get());
	}
	if (this->parent) {
		builder.parent(this->parent.get());
	}
	if (this->subtasks) {
		builder.subtasks(this->subtasks.get());
	}
	Task newTask = builder;
	if (this->completed) {
		newTask.setCompleted(this->completed.get());
	}
	#pragma endregion

	Log::debug << (boost::wformat(L"%1% : Updated to %2%") %
		logCategory % ToString(newTask)).str();

	return newTask;
}

void UpdateTask::updateDependencyGraph(State& state, const Task& task) const {
	Log::debug << (boost::wformat(L"%1% : Updating Dependencies %2% - \"%3%\"") %
		logCategory % task.getID() % task.getDescription()).str();
	Controller::Graph::updateTask(state.graph(), task);
}

void UpdateTask::updateSubtaskGraph(State& state, const Task& task) const {
	Log::debug << (boost::wformat(L"%1% : Updating Subtasks %2% - \"%3%\"") %
		logCategory % task.getID() % task.getDescription()).str();
	Controller::Graph::updateTask(state.sgraph(), task);
}

void UpdateTask::makeTransaction(const Task& updated) const {
	Log::debug << (boost::wformat(L"%1% : Commiting %2%") %
		logCategory % id).str();
	auto serialized = Controller::Serializer::serialize(updated);
	Transaction t(DataStore::get().begin());
	DataStore::get().put(this->id, serialized);
	t.commit();
}

void UpdateTask::recMarkChildren(const State& state,
	Task::ID id) const {
	auto task = state.graph().getTask(id);
	if (!task.getDependencies().empty()) {
		for (const auto& id : task.getDependencies()) {
			recMarkChildren(state, id);
		}
	}
	if (!task.getSubtasks().empty()) {
		for (const auto& id : task.getSubtasks()) {
			recMarkChildren(state, id);
		}
	}
	task.setCompleted(completed.get());
	Controller::Graph::updateTask(state.graph(), task);
	Controller::Graph::updateTask(state.sgraph(), task);
	makeTransaction(task);
}

void UpdateTask::markAllChildren(const State& state) const {
	assert(completed);
	recMarkChildren(state, id);
}

void UpdateTask::addAsSubtask(State& state) const {
	auto parentTask = state.graph().getTask(parent.get());
	auto newSubtasks = parentTask.getSubtasks();
	newSubtasks.insert(id);
	parentTask.setSubtasks(newSubtasks);
	UpdateTask(parentTask).execute(state);
	if (!previous.isTopLevel()) {
		auto oldParentTask =
			state.graph().getTask(previous.getParent());
		auto newSubtasks = oldParentTask.getSubtasks();
		newSubtasks.erase(previous.getID());
		oldParentTask.setSubtasks(newSubtasks);
		UpdateTask(oldParentTask).execute(state);
	}
}

Response UpdateTask::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : PUT %2%") %
		logCategory % id).str();
	previous = state.graph().getTask(id);
	auto updated = buildUpdatedTask(state);
	if (completed && (previous.isCompleted() != completed.get())) {
		markAllChildren(state);
	}
	if (parent && previous.getParent() != parent.get()) {
		addAsSubtask(state);
	}
	updateDependencyGraph(state, updated);
	updateSubtaskGraph(state, updated);
	makeTransaction(updated);
	return updated;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

//@author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"
#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/transaction.h"

#include "../state.h"
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
	return std::unique_ptr<Query>(
		new UpdateTask(previous));
}

Task UpdateTask::buildUpdatedTask(State& state) const {
	auto current = state.get().graph().getTask(this->id);
	auto builder = Controller::Builder::fromTask(current);

	#pragma region Update the fields iff it is requested
	if (this->description) {
		builder.description(this->description.get());
	}
	if (this->startTime) {
		builder.deadline(this->startTime.get());
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
	if (this->attachment) {
		builder.attachment(this->attachment.get());
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
	DataStore::get().put(updated.getID(), serialized);
	t.commit();
}

void UpdateTask::recMarkChildren(State& state,
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

void UpdateTask::reparentTask(State& state, Task::ID id,
	Task::ID newParent) const {
	auto theTask = state.graph().getTask(id);

	if (!theTask.isTopLevel()) {
		auto oldParentTask = state.graph().getTask(theTask.getParent());
		auto newSubtasks = oldParentTask.getSubtasks();
		newSubtasks.erase(id);
		oldParentTask.setSubtasks(newSubtasks);
		Controller::Graph::updateTask(state.graph(), oldParentTask);
		Controller::Graph::updateTask(state.sgraph(), oldParentTask);
		makeTransaction(oldParentTask);
	}

	{  // NOLINT(whitespace/braces)
		theTask.setParent(newParent);
		Controller::Graph::updateTask(state.graph(), theTask);
		Controller::Graph::updateTask(state.sgraph(), theTask);
		makeTransaction(theTask);
	}
}

Response UpdateTask::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : PUT %2%") %
		logCategory % id).str();
	previous = state.graph().getTask(id);
	auto currentSubtasks = previous.getSubtasksObject();
	auto updated = buildUpdatedTask(state);
	// Has completed/uncompleted
	if (completed && (previous.isCompleted() != completed.get())) {
		recMarkChildren(state, id);
	}
	// Has new parent.
	if (parent && (previous.getParent() != parent.get())) {
		reparentTask(state, id, parent.get());
	}
	// Reparent every subtask.
	if (subtasks) {
		// Set removed subtasks as toplevel task.
		for (const auto& c : currentSubtasks) {
			bool isRemovedFromSubtask =
				subtasks.get().find(c.getID())
					== subtasks.get().end();
			if (isRemovedFromSubtask) {
				reparentTask(state, c.getID(), c.getID());
			}
		}
		// Reparent new subtasks
		for (auto cid : subtasks.get()) {
			reparentTask(state, cid, id);
		}
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

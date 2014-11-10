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

using Transaction = You::DataStore::Transaction;
using DataStore = You::DataStore::DataStore;
using Log = You::Utils::Log;

const std::wstring UpdateTask::logCategory =
	Query::logCategory + L"[UpdateTask]";

std::unique_ptr<Query> UpdateTask::getReverse() {
	return QueryEngine::UpdateTask(previous);
}

Task UpdateTask::buildUpdatedTask(State& state) const {
	auto current = state.get().graph().getTask(this->id);
	auto builder = Controller::Builder::fromTask(current);

	// Update the fields if it is requested
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
	auto theNewParent = state.graph().getTask(newParent);

	// Remove the task from old parent's subtasks list.
	if (!theTask.isTopLevel()) {
		auto oldParentTask = state.graph().getTask(theTask.getParent());
		auto newSubtasks = oldParentTask.getSubtasks();
		newSubtasks.erase(id);
		oldParentTask.setSubtasks(newSubtasks);
		Controller::Graph::updateTask(state.graph(), oldParentTask);
		Controller::Graph::updateTask(state.sgraph(), oldParentTask);
		makeTransaction(oldParentTask);
	}

	// Add the task to the newParent subtasks and update it.
	if (newParent != id) {
		auto subtasks = theNewParent.getSubtasks();
		subtasks.insert(id);
		theNewParent.setSubtasks(subtasks);
		Controller::Graph::updateTask(state.graph(), theNewParent);
		Controller::Graph::updateTask(state.sgraph(), theNewParent);
		makeTransaction(theNewParent);
	}

	// Set the parent field of the task and update it.
	{  // NOLINT(whitespace/braces)
		theTask.setParent(newParent);
		Controller::Graph::updateTask(state.graph(), theTask);
		Controller::Graph::updateTask(state.sgraph(), theTask);
		makeTransaction(theTask);
	}
}

void UpdateTask::setRemovedSubtasksAsTopLevel(State& state) const {
	for (const auto& c : previous.getSubtasks()) {
		bool isRemovedFromSubtask =
			subtasks->find(c) == subtasks->end();
		if (isRemovedFromSubtask) {
			reparentTask(state, c, c);
		}
	}
}

void UpdateTask::updateActiveFilter(State& state) const {
	state.setActiveFilter(
		state.getActiveFilter() ||
			Filter::idIsIn({ this->id }));
}

Response UpdateTask::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : PUT %2%") %
		logCategory % id).str();
	previous = state.graph().getTask(id);
	auto currentSubtasks = previous.getSubtasksObject();
	auto updated = buildUpdatedTask(state);
	try {
		// If a parent is changed, reparent the task accordingly.
		if (parent && (previous.getParent() != *parent)) {
			reparentTask(state, id, *parent);
		}
		updateDependencyGraph(state, updated);
		updateSubtaskGraph(state, updated);
		// If a task is marked as complete, mark all the
		// dependencies and subtasks as complete as well.
		if (completed && (previous.isCompleted() != *completed)) {
			recMarkChildren(state, id);
		}
		// If the subtasks field is changed, reparent every subtask as necessary.
		if (subtasks) {
			setRemovedSubtasksAsTopLevel(state);
			for (Task::ID cid : *subtasks) {
				reparentTask(state, cid, id);
			}
		}
		updateActiveFilter(state);
		makeTransaction(updated);
	} catch (Exception::CircularDependencyException&) {
		UpdateTask(previous).execute(state);
		return previous;
	}
	return updated;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

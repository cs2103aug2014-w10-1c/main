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

	return newTask;
}

void UpdateTask::updateDependencyGraph(State& state, const Task& task) const {
	Log::debug << (boost::wformat(L"%1% : Updating %2% - \"%3%\"\n") %
		logCategory % task.getID() % task.getDescription()).str();
	Controller::Graph::updateTask(state.graph(), task);
}

void UpdateTask::updateSubtaskGraph(State& state, const Task& task) const {
}

void UpdateTask::makeTransaction(const Task& updated) const {
	auto serialized = Controller::Serializer::serialize(updated);
	Transaction t(DataStore::get().begin());
	DataStore::get().put(this->id, serialized);
	t.commit();
}

Response UpdateTask::execute(State& state) {
	previous = state.graph().getTask(id);
	auto updated = buildUpdatedTask(state);
	updateDependencyGraph(state, updated);
	return updated;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

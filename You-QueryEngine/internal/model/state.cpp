#include "stdafx.h"

#include "../../../You-DataStore/datastore.h"
#include "../../../You-DataStore/task_typedefs.h"
#include "../controller/task_graph_controller.h"

#include "state.h"

namespace You {
namespace QueryEngine {
namespace Internal {

namespace {
	using DataStore = You::DataStore::DataStore;
	using Transaction = You::DataStore::Transaction;
	using KeyValuePairs = You::DataStore::KeyValuePairs;
}

const std::wstring State::MAX_ID_FIELD = L"max-id";

std::int64_t State::getMaxIDFromDataStore() {
	auto resources = DataStore::get().getAllResources();
	bool resourceFound = false;
	for (const auto& resource : resources) {
		auto it = resource.find(MAX_ID_FIELD);
		if (it != resource.end()) {
			resourceFound = true;
			auto newMaxID = boost::lexical_cast<Task::ID>(it->second);
			return newMaxID;
		}
	}
	return -1;
}

State::State()
: innerGraph(TaskGraph(TaskGraph::GraphType::DEPENDENCY)),
  innerSubtaskGraph(TaskGraph(TaskGraph::GraphType::SUBTASK)) {
	maxID = TaskGraphController::loadFromFile(innerGraph);
	TaskGraphController::loadFromFile(innerSubtaskGraph);
	commitMaxIDToDataStore();
}

State& State::get() {
	static State instance;
	return instance;
}

void State::clear() {
	get().innerGraph = TaskGraph(TaskGraph::GraphType::DEPENDENCY);
	get().innerSubtaskGraph = TaskGraph(TaskGraph::GraphType::SUBTASK);
	while (!get().undoStack().empty()) {
		get().undoStack().pop();
	}
	get().maxID = 0;
}

void State::setActiveFilter(const Filter& filter) {
	this->activeFilter = filter;
}

void State::setActiveComparator(const Comparator& comparator) {
	this->activeComparator = comparator;
}

Task::ID State::inquireNewID() {
	++maxID;
	return maxID;
}

void State::commitMaxIDToDataStore() {
	Transaction t(DataStore::get().begin());
	auto previousMaxID = getMaxIDFromDataStore();
	maxID = std::max(previousMaxID, maxID);
	auto sMaxID = boost::lexical_cast<std::wstring>(maxID);
	if (previousMaxID == -1) {
		DataStore::get().post(MAX_ID_FIELD, {{ MAX_ID_FIELD, sMaxID }});
	} else {
		DataStore::get().put(MAX_ID_FIELD, {{ MAX_ID_FIELD, sMaxID }});
	}
	t.commit();
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

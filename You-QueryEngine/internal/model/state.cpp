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
}

const std::wstring State::MAX_ID_FIELD = L"max-id";

State::State() {
	innerGraph = TaskGraph();
	maxID = TaskGraphController::loadFromFile(innerGraph);
	auto resources = DataStore::get().getAllResources();
	bool resourceFound = false;
	// Search resource list for max id.
	for (const auto& resource : resources) {
		auto it = resource.find(MAX_ID_FIELD);
		if (it != resource.end()) {
			resourceFound = true;
			auto newMaxID = boost::lexical_cast<Task::ID>(it->second);
			// The check is neccessary to handle ungraceful
			// program termination (maxID might not be stored)
			if (newMaxID > maxID) {
				maxID = newMaxID;
			}
			break;
		}
	}

	// If MAX_ID is missing in the resource file
	if (!resourceFound) {
		commitMaxIDToDataStore(true);
	}
}

State& State::get() {
	static State instance;
	return instance;
}

void State::clear() {
	get().innerGraph = TaskGraph();
	while (!get().undoStack().empty()) {
		get().undoStack().pop();
	}
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

void State::commitMaxIDToDataStore(bool isFirstTime) {
	auto sMaxID = boost::lexical_cast<std::wstring>(maxID);
	Transaction t(DataStore::get().begin());
	if (isFirstTime) {
		DataStore::get().post(MAX_ID_FIELD, { { MAX_ID_FIELD, sMaxID } });
	} else {
		DataStore::get().put(MAX_ID_FIELD, { { MAX_ID_FIELD, sMaxID } });
	}
	t.commit();
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

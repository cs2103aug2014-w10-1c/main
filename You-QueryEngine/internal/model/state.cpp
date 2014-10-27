#include "stdafx.h"
#include "state.h"

#include "../controller/task_graph_controller.h"

namespace You {
namespace QueryEngine {
namespace Internal {

State::State()
: innerGraph(TaskGraph(TaskGraph::GraphType::DEPENDENCY)),
  innerSubtaskGraph(TaskGraph(TaskGraph::GraphType::SUBTASK)) {
	maxID = TaskGraphController::loadFromFile(innerGraph);
	TaskGraphController::loadFromFile(innerSubtaskGraph);
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

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

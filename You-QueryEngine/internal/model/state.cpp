#include "stdafx.h"
#include "state.h"

#include "../controller/task_graph_controller.h"

namespace You {
namespace QueryEngine {
namespace Internal {

State::State() {
	innerGraph = TaskGraph();
	maxID = TaskGraphController::loadFromFile(innerGraph);
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

Task::ID State::inquireNewID() {
	++maxID;
	return maxID;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

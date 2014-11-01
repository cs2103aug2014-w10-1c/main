/// \file You-QueryEngine/internal/model/state.h
/// Defines the State class.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_MODEL_STATE_H_
#define YOU_QUERYENGINE_INTERNAL_MODEL_STATE_H_

#include <stack>
#include <memory>
#include "../../api.h"
#include "task_graph.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Singleton class to store \ref TaskGraph instance.\n
/// When the application run, only one TaskGraph instance
/// is alive. This singleton property of this class helped
/// to achieve this property.
class State {
public:
	/// Getter of the current instance.
	static State& get();

	/// Get the task dependency graph.
	inline TaskGraph& graph() const { return get().innerGraph;  }

	/// Get the task subtask graph.
	inline TaskGraph& sgraph() const { return get().innerSubtaskGraph;  }

	/// Get the current active filter.
	inline Filter& getActiveFilter() const { return get().activeFilter; }

	/// Get the current active comparator.
	inline Comparator& getActiveComparator() const {
		return get().activeComparator; }

	/// Update the active filter
	void setActiveFilter(const Filter& filter);

	/// Update the active comparator
	void setActiveComparator(const Comparator& comparator);

	/// Reset the state back to empty state. \n
	/// Should be used only if necessary.
	static void clear();

	/// Get the undo stack.
	inline std::stack<std::unique_ptr<Query>>& undoStack() {
		return get().innerUndoStack; }

	/// Inquire a new and unique task id.
	Task::ID inquireNewID();

	/// Get max id from datastore.
	/// Return -1 if not found.
	std::int64_t getMaxIDFromDataStore();

	/// Save max id to datastore.
	void commitMaxIDToDataStore();

private:
	State();
	State(const State&) = delete;
	State& operator=(const State&) = delete;

	Task::ID maxID;
	TaskGraph innerGraph;
	TaskGraph innerSubtaskGraph;
	std::stack<std::unique_ptr<Query>> innerUndoStack;

	static const std::wstring MAX_ID_FIELD;
	Filter activeFilter = Filter::anyTask();
	Comparator activeComparator = Comparator::notSorted();
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_MODEL_STATE_H_

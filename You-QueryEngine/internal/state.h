/// \file You-QueryEngine/internal/state.h
/// Defines the State class.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_STATE_H_
#define YOU_QUERYENGINE_INTERNAL_STATE_H_

#include <stdexcept>
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

	/// Reset the state back to empty state. \n
	/// Should be used only if necessary.
	static void clear();

	/// Get the task graph.
	inline TaskGraph& graph() const { return get().innerGraph;  }

	/// Inquire a new and unique task id.
	Task::ID inquireNewID();

private:
	State() : innerGraph(std::move(TaskGraph())) {}
	State(const State&) = delete;
	State& operator=(const State&) = delete;

	TaskGraph innerGraph;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_STATE_H_

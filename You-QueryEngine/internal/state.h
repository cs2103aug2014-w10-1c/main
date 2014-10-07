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

/// Static class to store \ref DataStore and \ref TaskGraph instances.
class State {
public:
	/// Get the current instance.
	static State& get();

	/// Reset the state to empty.
	/// Be careful!
	static void clear();

	/// Get the graph of the current instance
	inline TaskGraph& graph() const { return get().innerGraph;  }

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

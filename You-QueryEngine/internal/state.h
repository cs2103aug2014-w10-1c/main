#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_STATE_H_
#define YOU_QUERYENGINE_INTERNAL_STATE_H_

#include <stack>
#include "../query.h"

namespace You {
namespace QueryEngine {

/// Define a singleton query engine state
/// Instantiated when application starts
class State {
public:
	/// Get the instance of the singleton state
	State& get();

	~State() = default;
	State(const State&) = delete;
	State& operator=(const State&) = delete;

private:
	State() = default;

	/// Undo stack
	std::stack<Query::Query> undoStack;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_STATE_H_

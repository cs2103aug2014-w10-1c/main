#include "stdafx.h"
#include "state.h"

namespace You {
namespace QueryEngine {
namespace Internal {

State& State::get() {
	static State instance;
	return instance;
}

void State::clear() {
	get().innerGraph = TaskGraph();
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

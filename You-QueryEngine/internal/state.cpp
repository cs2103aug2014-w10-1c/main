#include "stdafx.h"
#include "state.h"

namespace You {
namespace QueryEngine {
namespace Internal {

State& State::get() {
	static State instance;
	return instance;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

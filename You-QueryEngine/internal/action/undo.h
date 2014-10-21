/// \file undo.h
/// Defines action for undoing last actions.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_UNDO_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_UNDO_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Action for updating task (e.g marking it as done)
class Undo : public Query {
public:
	/// Construct EditTask query
	Undo();

	/// Disable assignment operator
	Undo& operator=(const Undo&) = delete;

	/// Destructor
	virtual ~Undo() = default;

	/// Execute add task.
	Response execute(State& tasks) override;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_UNDO_H_

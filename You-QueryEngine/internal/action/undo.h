/// \file undo.h
/// Defines action for undoing last actions.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_UNDO_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_UNDO_H_

#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Undo last action, then reapply the current filter and comparator
/// and return the task list as a response.
/// - The undo of add task is delete task and vice versa.
/// - The undo of update task is update the task with the old one.
class Undo : public Query {
public:
	/// Construct Undo query
	Undo() = default;

	/// Destructor
	virtual ~Undo() = default;
private:
	Undo& operator=(const Undo&) = delete;

	Response execute(State& tasks) override;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_UNDO_H_

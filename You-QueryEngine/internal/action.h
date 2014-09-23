#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_H_

#include "exception.h"

namespace You {
namespace QueryEngine {

/// \class Action
/// Defines an interface for all You command (addTask, deleteTask, etc.)
template<class TCommand, class TData, class TResponse>
class IAction {
public:
	/// Read from a command and modify internal state.
	virtual TResponse read(const TCommand&);

	/// Execute the action and mutate data.
	virtual TResponse execute(TData&) = 0;

	/// Execute without mutating data.
	virtual TResponse execute_(const TData&) = 0;

	/// Get the reverse of this action.
	virtual IAction getReverse() = 0;

	virtual ~Action() = default;
	Action() = default;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_H_

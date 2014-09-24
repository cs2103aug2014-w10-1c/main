/// \file action.h
/// Defines the action interface.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_H_

#include "query.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// An interface for all You command (addTask, deleteTask, etc.)
/// Example usage
/// \code {.cpp}
///   class AddTask : public IAction<ADD_QUERY, TaskList, Task>;
/// \endcode
/// \param TQuery The type of the query, \see internal/schema.h
/// \param TData The type of the data to be executed upon.
/// \param TResponse The type of the response. \see query.h
template<class TQuery, class TData, class TResponse>
class IAction {
public:
	/// Read from a query and modify internal state.
	/// \param [in] unnamed Query to be read
	virtual void read(const TQuery&);

	/// Execute and mutate data.
	/// \param [in] unnamed The mutable data to be executed upon.
	/// \returns A response object of type TResponse
	virtual TResponse execute(TData&) = 0;

	/// Execute without mutating data.
	/// \param [in]	unnamed The immutable data to be executed upon.
	/// \returns A response object of type TResponse
	virtual TResponse execute_(const TData&) const = 0;

	/// Get a query as a representation of the reverse of this action.
	virtual Query::Query getReverse() const = 0;

	virtual ~Action() = default;
	Action() = default;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_H_

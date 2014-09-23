#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_H_

namespace You {
namespace QueryEngine {

/// Defines an interface for all You command (addTask, deleteTask, etc.)
/// Example usage
/// \code {.cpp}
///   class AddTask : public IAction<ADD_QUERY, TaskList, Task>;
/// \endcode
template<class TQuery, class TData, class TResponse>
class IAction {
public:
	/// Read from a query and modify internal state.
	/// \param [TQuery&] Query to be read
	virtual void read(const TQuery&);

	/// Execute and mutate data.
	/// \param [TData]	The mutable data to be executed upon.
	/// \return A response object of type TResponse
	virtual TResponse execute(TData&) = 0;

	/// Execute without mutating data.
	/// \param [TData&]	The immutable data to be executed upon.
	/// \return A response object of type TResponse
	virtual TResponse execute_(const TData&) const = 0;

	/// Get the reverse of this action.
	virtual IAction getReverse() const = 0;

	virtual ~Action() = default;
	Action() = default;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_H_

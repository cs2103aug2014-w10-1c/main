/// \file get_task.h
/// Defines action for getting a list of tasks by applying
/// a filter and a sorter.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_GET_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_GET_TASK_H_

#include <functional>
#include "../../filter.h"
#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

/// Action for getting tasks by applying a filter and optionally a sorter.
class GetTask : public Query {
public:
	/// Construct from filter, do not sort.
	/// \param [in] filter The filter used.
	explicit GetTask(const Filter& filter)
	: filter(filter) {}

	/// Construct from filter and sort.
	/// \param [in] filter The filter used.
	/// \param [in] comparator The comparator used.
	explicit GetTask(const Filter& filter, const Comparator& comparator)
	: filter(filter), comparator(comparator), sortAfterFilter(true) {}

	/// Destructor
	virtual ~GetTask() = default;

protected:
	/// The header of the log string
	static const std::wstring logCategory;

private:
	Response execute(State& tasks) override;
	GetTask& operator=(const GetTask&) = delete;
	Filter filter;
	Comparator comparator = Comparator::notSorted();
	bool sortAfterFilter = false;
};

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_GET_TASK_H_

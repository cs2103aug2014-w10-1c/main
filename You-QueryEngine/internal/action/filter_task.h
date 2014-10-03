/// \file update_task.h
/// Defines action for updating tasks.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_FILTER_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_FILTER_TASK_H_

#include <functional>
#include "../../filter.h"
#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Define action for updating task
class FilterTask : public Query {
public:
	/// Constructor
	explicit FilterTask(const Filter& filter) : filter(filter) {}

private:
	/// Execute add task.
	Response execute(State& const tasks) override;
	FilterTask& operator=(const FilterTask&) = delete;
	const Filter& filter;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_FILTER_TASK_H_

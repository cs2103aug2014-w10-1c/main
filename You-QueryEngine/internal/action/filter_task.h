/// \file update_task.h
/// Defines action for updating tasks.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_ACTION_FILTER_TASK_H_
#define YOU_QUERYENGINE_INTERNAL_ACTION_FILTER_TASK_H_

#include <functional>
#include "../../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Define action for updating task
class FilterTask : public Query {
public:
	/// Type definition of a filter
	typedef std::function<bool(const Task&)> Filter;

	/// Constructor
	explicit FilterTask(const Filter& filter) : filter(filter) {}

	/// Disable assignment operator
	FilterTask& operator=(const FilterTask&) = delete;

private:
	/// Execute add task.
	Response execute() override;

	const Filter filter;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_ACTION_FILTER_TASK_H_

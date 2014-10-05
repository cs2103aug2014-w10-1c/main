/// \author A0112054Y
#include "stdafx.h"

#include "internal/action/add_task.h"
#include "internal/action/filter_task.h"
#include "api.h"

namespace You {
namespace QueryEngine {

std::unique_ptr<Query>
FilterTask(const Filter& filter) {
	using FilterTask = Internal::FilterTask;
	return std::unique_ptr<Query>(new FilterTask(filter));
}

std::unique_ptr<Query>
AddTask(Task::Description description, Task::Time deadline,
	Task::Priority priority, Task::Dependencies dependencies) {
	using AddTask = Internal::AddTask;
	return std::unique_ptr<Query>(new AddTask(description, deadline,
		priority, dependencies));
}

Response executeQuery(std::unique_ptr<Query> query) {
	State blank;
	return query->execute(blank);
}

Response executeQuery(std::unique_ptr<Query> query, State& state) {
	return query->execute(state);
}

}  // namespace QueryEngine
}  // namespace You


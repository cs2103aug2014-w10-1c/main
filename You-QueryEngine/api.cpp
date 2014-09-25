#include "stdafx.h"

#include "api.h"
#include "internal/action/add_task.h"

namespace You {
namespace QueryEngine {

/// Construct a new add task action
std::unique_ptr<Query>
ADD_TASK(Task::Description description, Task::Time deadline,
	Task::Priority priority, Task::Dependencies dependencies) {
	using AddTask = Internal::AddTask;
	return std::unique_ptr<Query>(new AddTask(description, deadline,
		priority, dependencies));
}

/// Execute the query and return a response
Response executeQuery(std::unique_ptr<Query> query) {
	return query->execute();
}

}  // namespace QueryEngine
}  // namespace You


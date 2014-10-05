/// \author A0112054Y
#include "stdafx.h"

#include  "../task_serializer.h"
#include "filter_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Response FilterTask::execute(State& const tasks) {
	std::vector<Task> result;
	auto filter = this->filter;
	std::for_each(tasks.cbegin(), tasks.cend(),
		[filter, &result] (const Task task) {
			if (filter(task)) {
				result.push_back(task);
			}
		}
	);
	return result;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

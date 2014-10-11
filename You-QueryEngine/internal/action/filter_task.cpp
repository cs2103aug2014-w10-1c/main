/// \author A0112054Y
#include "stdafx.h"

#include  "../model.h"
#include  "../controller.h"
#include "filter_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

Response FilterTask::execute(State& state) {
	std::vector<Task> result;
	auto filter = this->filter;
	std::vector<Task> all = state.graph().asTaskList();
	std::for_each(all.begin(), all.end(),
		[filter, &result] (const Task task) {
			if (filter(task)) {
				result.push_back(task);
			}
		}
	);
	return result;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

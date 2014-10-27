/// \author A0112054Y
#include "stdafx.h"

#include "../model.h"
#include "../controller.h"
#include "get_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

const std::wstring GetTask::logCategory = Query::logCategory + L"[GetTask]";

Response GetTask::execute(State& state) {
	std::vector<Task> result;
	std::vector<Task> all = state.graph().asTaskList();
	for (const auto& task : all) {
		if (filter(task)) {
			result.push_back(task);
		}
	}
	if (sortAfterFilter) {
		std::sort(begin(result), end(result), comparator);
		state.setActiveComparator(comparator);
	} else {
		state.setActiveComparator(Comparator::notSorted());
	}
	state.setActiveFilter(filter);
	return result;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

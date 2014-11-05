//@author A0112054Y
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
	std::vector<Task> filtered;
	std::vector<Task> result;
	std::vector<Task> all = state.graph().asTaskList();
	std::unordered_set<Task::ID> filteredID;
	for (const auto& task : all) {
		if (filter(task)) {
			filtered.push_back(task);
			filteredID.insert(task.getID());
		}
	}
	for (const auto& r : filtered) {
		// Always show toplevel task.
		if (r.isTopLevel()) {
			result.push_back(r);
		} else {
			// Show child task iff the parent is not filtered.
			auto parent = r.getParent();
			bool parentIsAlreadyFiltered =
				filteredID.find(parent) != filteredID.end();
			if (!parentIsAlreadyFiltered) {
				result.push_back(r);
			}
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

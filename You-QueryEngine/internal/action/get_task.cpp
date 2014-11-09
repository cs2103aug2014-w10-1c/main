//@author A0112054Y
#include "stdafx.h"

#include "../state.h"
#include "../controller.h"
#include "get_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

const std::wstring GetTask::logCategory = Query::logCategory + L"[GetTask]";

std::unordered_map<Task::ID, Task>
GetTask::getFilteredTasks(const State& state) const {
	std::unordered_map<Task::ID, Task> filtered;
	std::vector<Task> all = state.graph().asTaskList();
	for (const auto& task : all) {
		if (filter(task)) {
			filtered.insert({ task.getID(), task });
		}
	}
	return filtered;
}

std::vector<Task> GetTask::removeTaskIfParentIsShown(
	std::unordered_map<Task::ID, Task>& filtered) const {
	std::vector<Task> result;
	for (const auto& r : filtered) {
		// Always show toplevel task.
		if (r.second.isTopLevel()) {
			result.push_back(r.second);
		} else {
			// Show child task iff the parent is not filtered.
			auto parent = r.second.getParent();
			bool parentIsAlreadyFiltered =
				filtered.find(parent) != filtered.end();
			if (!parentIsAlreadyFiltered) {
				result.push_back(r.second);
			}
		}
	}
	return result;
}

void GetTask::sortTheResultIfRequested(std::vector<Task>& result) const {
	if (sortAfterFilter) {
		std::sort(begin(result), end(result), comparator);
	} else {
		std::sort(begin(result), end(result), Comparator::byTimeCreated());
	}
}

void GetTask::updateActiveFilterAndSorter(State& state) const {
	state.setActiveComparator(comparator);
	state.setActiveFilter(filter);
}

Response GetTask::execute(State& state) {
	std::unordered_map<Task::ID, Task> filtered = getFilteredTasks(state);
	std::vector<Task> result = removeTaskIfParentIsShown(filtered);
	sortTheResultIfRequested(result);
	updateActiveFilterAndSorter(state);
	return result;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

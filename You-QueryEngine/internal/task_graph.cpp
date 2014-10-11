#include "stdafx.h"
#include "task_graph.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Task TaskGraph::getTask(const Task::ID id) const {
	auto get = taskTable.find(id);
	if (get != taskTable.end()) {
		return get->second;
	} else {
		throw Exception::TaskNotFoundException();
	}
}

std::vector<Task> TaskGraph::asTaskList() const {
	std::vector<Task> result;
	for (auto pair = taskTable.cbegin(); pair != taskTable.cend();
			++pair) {
		result.push_back(pair->second);
	}
	return result;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

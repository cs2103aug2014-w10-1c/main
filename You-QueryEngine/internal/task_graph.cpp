//@author A0112054Y
#include "stdafx.h"
#include "../exception.h"
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
	for (const auto& idTaskPair : taskTable) {
		result.push_back(idTaskPair.second);
	}
	return result;
}

Task::Dependencies TaskGraph::getAdjacentTasks(const Task& task) const {
	if (type == TaskGraph::GraphType::DEPENDENCY) {
		return task.getDependencies();
	} else {
		return task.getSubtasks();
	}
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

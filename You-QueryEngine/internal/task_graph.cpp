#include "stdafx.h"
#include "exception.h"
#include "task_graph.h"

namespace You {
namespace QueryEngine {
namespace Internal {

void TaskGraph::addTask(const Task& task) {
	Vertex v = boost::add_vertex(graph);
	graph[v] = task.getID();
	taskTable.insert({ task.getID(), task });
}

void TaskGraph::deleteTask(const Task::ID id) {
	VIterator begin;
	VIterator end;
	VIterator next;
	bool removed = false;
	boost::tie(begin, end) = boost::vertices(graph);
	for (next = begin; !removed && next != end; begin = next) {
		const Task::ID idInVertex = graph[*next];
		if (idInVertex == id) {
			removed = true;
			boost::remove_vertex(*begin, graph);
		}
		++next;
	}
	if (!removed) {
		throw TaskNotFoundException();
	} else {
		taskTable.erase(id);
	}
}

Task TaskGraph::getTask(const Task::ID id) {
	try {
		Task get = taskTable.at(id);
	} catch (const std::out_of_range&) {
		throw TaskNotFoundException();
	}
}

std::vector<Task> TaskGraph::getTaskList() const {
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

#include "stdafx.h"
#include "exception.h"
#include "task_graph.h"

namespace You {
namespace QueryEngine {
namespace Internal {

void TaskGraph::addTask(const Task& task) {
	Vertex v = boost::add_vertex(graph);
	graph[v] = (Task) Internal::TaskBuilder::fromTask(task);
}

void TaskGraph::deleteTask(const Task::ID id) {
	VIterator begin;
	VIterator end;
	VIterator next;
	bool removed = false;
	boost::tie(begin, end) = boost::vertices(graph);
	for (next = begin; !removed && next != end; begin = next) {
		const Task::ID idInVertex = graph[*next].getID();
		if (idInVertex == id) {
			removed = true;
			boost::remove_vertex(*begin, graph);
		}
		++next;
	}
	if (!removed) {
		throw TaskNotFoundException();
	}
}

std::vector<Task> TaskGraph::getTaskList() const {
	std::vector<Task> result;
	VIterator begin;
	VIterator end;
	VIterator next;
	boost::tie(begin, end) = boost::vertices(graph);
	for (next = begin; next != end; ++next) {
		result.push_back(graph[*next]);
	}
	return result;
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

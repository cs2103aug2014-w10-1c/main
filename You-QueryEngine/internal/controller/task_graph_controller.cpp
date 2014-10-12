#include "stdafx.h"
#include "task_graph_controller.h"

#include "task_serializer.h"
#include "../../../You-DataStore/datastore.h"

namespace You {
namespace QueryEngine {
namespace Internal {

using TGC = TaskGraphController;
using Vertex = TaskGraph::Vertex;
using VIterator = TaskGraph::VIterator;

Task::ID TGC::loadFromFile(TaskGraph& graph) {
	auto serialized =
		You::DataStore::DataStore::get().getAllTasks();
	Task::ID maxID = 0;
	std::for_each(serialized.cbegin(), serialized.cend(),
		[&] (const TaskSerializer::STask task) {
			auto t = TaskSerializer::deserialize(task);
			addTask(graph, t);
			maxID = std::max(t.getID(), maxID);
		}
	);
	return maxID;
}

void TGC::addTask(TaskGraph& g, const Task& task) {
	Vertex v = boost::add_vertex(g.graph);
	g.graph[v] = task.getID();
	g.taskTable.insert({ task.getID(), task });
}

void TGC::deleteTask(TaskGraph& g, const Task::ID id) {
	VIterator begin;
	VIterator end;
	VIterator next;
	bool removed = false;
	boost::tie(begin, end) = boost::vertices(g.graph);
	for (next = begin; !removed && next != end; begin = next) {
		const Task::ID idInVertex = g.graph[*next];
		if (idInVertex == id) {
			removed = true;
			boost::remove_vertex(*begin, g.graph);
		}
		++next;
	}
	if (!removed) {
		throw Exception::TaskNotFoundException();
	} else {
		g.taskTable.erase(id);
		rebuildGraph(g);
	}
}

void TGC::updateTask(TaskGraph& g, const Task& task) {
	auto found = g.taskTable.find(task.getID());
	if (found != g.taskTable.end()) {
		found->second = task;
		rebuildGraph(g);
	} else {
		throw Exception::TaskNotFoundException();
	}
}

void TGC::rebuildGraph(TaskGraph& g) {
	g.graph = TaskGraph::Graph();
	for (auto pair = g.taskTable.cbegin(); pair != g.taskTable.cend();
		++pair) {
		Vertex v = boost::add_vertex(g.graph);
		g.graph[v] = pair->first;
	}
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

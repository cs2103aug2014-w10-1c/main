#include "stdafx.h"

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>
#include "task_serializer.h"
#include "../../../You-DataStore/datastore.h"

#include "task_graph_controller.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// \cond Shorten names
namespace {
	using TGC = TaskGraphController;
	using Vertex = TaskGraph::Vertex;
	using VIterator = TaskGraph::VIterator;
}
/// \endcond

Task::ID TGC::loadFromFile(TaskGraph& graph) {
	Task::ID maxID = 0;
	auto serialized =
		You::DataStore::DataStore::get().getAllTasks();
	for (const auto& task : serialized) {
		auto t = TaskSerializer::deserialize(task);
		addTask(graph, t);
		maxID = std::max(t.getID(), maxID);
	}
	return maxID;
}

bool TGC::isTaskExist(TaskGraph& graph, const Task::ID id) {
	bool isExist = true;
	try {
		graph.getTask(id);
	} catch (const Exception::TaskNotFoundException&) {
		isExist = false;
	}
	return isExist;
}

void TGC::addTask(TaskGraph& g, const Task& task) {
	boost::add_vertex(task.getID(), g.graph);
	g.taskTable.insert({ task.getID(), task });
	if (task.getDependencies().size() > 0) {
		addAllDependencies(g, task);
	}
}

void TGC::addAllDependencies(TaskGraph& g, const Task& task) {
	for (const auto& cid : task.getDependencies()) {
		 addDependency(g, task.getID(), cid);
	}
}

void TGC::addDependency(TaskGraph& g, const Task::ID pid, const Task::ID cid) {
	boost::add_edge(cid, pid, g.graph);
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
		bool dependencyIsChanged =
			task.getDependencies() != (found->second).getDependencies();
		if (dependencyIsChanged) {
			auto backup = found->second;
			found->second = task;
			bool hasCycle = false;
			CycleDetector detector(hasCycle);
			rebuildGraph(g);
			boost::depth_first_search(g.graph, boost::visitor(detector));
			if (hasCycle) {
				found->second = backup;
				rebuildGraph(g);
				throw Exception::CircularDependencyException();
			}
		} else {
			found->second = task;
		}
	} else {
		throw Exception::TaskNotFoundException();
	}
}

void TGC::rebuildGraph(TaskGraph& g) {
	g.graph = TaskGraph::Graph();
	for (const auto& idTaskPair : g.taskTable) {
		Vertex v = boost::add_vertex(g.graph);
		g.graph[v] = idTaskPair.first;
	}
	for (const auto& idTaskPair : g.taskTable) {
		for (const auto& cid : idTaskPair.second.getDependencies()) {
			boost::add_edge(cid, idTaskPair.first, g.graph);
		}
	}
}

/// \endcond
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

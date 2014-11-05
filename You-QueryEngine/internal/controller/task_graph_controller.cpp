#include "stdafx.h"

#include "../../../You-DataStore/datastore.h"
#include "task_serializer.h"

#include "task_graph_controller.h"

namespace You {
namespace QueryEngine {
namespace Internal {

using TGC = TaskGraphController;
using Vertex = TaskGraph::Vertex;
using Graph = TaskGraph::Graph;
using VIterator = TaskGraph::VIterator;
using DataStore = You::DataStore::DataStore;

Task::ID TaskGraphController::loadFromFile(TaskGraph& graph) {
	Task::ID maxID = 0;
	auto serialized = DataStore::get().getAllTasks();
	for (const auto& task : serialized) {
		auto t = TaskSerializer::deserialize(task);
		addTask(graph, t);
		maxID = std::max(t.getID(), maxID);
	}
	return maxID;
}

bool TaskGraphController::isTaskExist(TaskGraph& graph,
	const Task::ID id) {
	bool isExist = true;
	try {
		graph.getTask(id);
	} catch (const Exception::TaskNotFoundException&) {
		isExist = false;
	}
	return isExist;
}

void TaskGraphController::addTask(TaskGraph& g, const Task& task) {
	assert(!isTaskExist(g, task.getID()));
	auto neighbors = g.getAdjacentTasks(task);
	bool noCycleInNeighbors =
		std::all_of(begin(neighbors), end(neighbors),
			std::bind(std::not_equal_to<Task::ID>(),
				task.getID(), std::placeholders::_1));
	if (noCycleInNeighbors) {
		boost::add_vertex(task.getID(), g.graph);
		g.taskTable.insert({ task.getID(), task });
		if (neighbors.size() > 0) {
			connectEdges(g, task);
		}
	} else {
		throw Exception::CircularDependencyException();
	}
}

void TaskGraphController::connectEdges(TaskGraph& g, const Task& task) {
	auto neighbors = g.getAdjacentTasks(task);
	for (const auto& cid : neighbors) {
		 connectEdge(g, task.getID(), cid);
	}
}

void TaskGraphController::connectEdge(TaskGraph& g,
	const Task::ID pid, const Task::ID cid) {
	boost::add_edge(cid, pid, g.graph);
}

void TaskGraphController::deleteTask(TaskGraph& g, const Task::ID id) {
	Task task;

	try {
		task = g.getTask(id);
	} catch (const Exception::TaskNotFoundException& e) {
		return;
	}

	/// Reparent the graph
	if (g.type == TaskGraph::GraphType::DEPENDENCY) {
		auto children = task.getDependencies();
		for (auto& t : g.asTaskList()) {
			if (t.isDependOn(task.getID())) {
				auto dep = t.getDependencies();
				for (const auto& c : children) {
					dep.insert(c);
				}
				t.setDependencies(dep);
				g.taskTable[t.getID()] = t;
			}
		}
	}

	g.taskTable.erase(id);
}

void TaskGraphController::updateTask(TaskGraph& g, const Task& task) {
	auto found = g.taskTable.find(task.getID());
	if (found != g.taskTable.end()) {
		auto neighborBefore = g.getAdjacentTasks(task);
		auto neighborAfter = g.getAdjacentTasks(found->second);
		bool neighborIsChanged = neighborBefore != neighborAfter;
		if (neighborIsChanged) {
			auto backup = found->second;
			found->second = task;
			bool hasCycle = false;
			CycleDetector detector(hasCycle);
			rebuildGraph(g);
			boost::depth_first_search(g.graph,
				boost::visitor(detector));
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

void TaskGraphController::rebuildGraph(TaskGraph& g) {
	g.graph = TaskGraph::Graph();
	for (const auto& idTaskPair : g.taskTable) {
		Vertex v = boost::add_vertex(g.graph);
		g.graph[v] = idTaskPair.first;
	}
	for (const auto& idTaskPair : g.taskTable) {
		for (const auto& cid : g.getAdjacentTasks(
			idTaskPair.second)) {
			boost::add_edge(cid, idTaskPair.first, g.graph);
		}
	}
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

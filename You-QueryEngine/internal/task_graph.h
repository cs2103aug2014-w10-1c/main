/// \file task_graph.h
/// Defines the TaskGraph class
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_H_

#include <vector>
#include <stdexcept>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include "exception.h"
#include "../task_model.h"

namespace You {
namespace QueryEngine {
namespace Internal {

class TaskGraphController;

/// Defines a simple task graph
class TaskGraph {
	friend class TaskGraphController;
public:
	/// TaskGraph is a directed graph of task.
	typedef boost::adjacency_list<boost::vecS, boost::vecS,
		boost::directedS, Task::ID> Graph;
	/// Type of the vertices
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	/// Type of the vertex iterator
	typedef boost::graph_traits<Graph>::vertex_iterator VIterator;

	/// Retrieve a single task from the graph
	/// May throw Exception::TaskNotFoundException
	/// \param [in] id The id of the task to be retrieved.
	/// \return The task with id \ref id
	Task getTask(const Task::ID id) const;

	/// Represent the graph as list of tasks.
	/// \return Vector of all tasks.
	std::vector<Task> asTaskList() const;

	/// Return number of task resided in the graph.
	inline int getTaskCount() const { return taskTable.size(); }

private:
	Graph graph;
	std::unordered_map<Task::ID, Task> taskTable;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_H_

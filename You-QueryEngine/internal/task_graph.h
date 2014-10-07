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
#include "task_builder.h"
#include "../task_model.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Defines a simple task graph
class TaskGraph {
public:
	/// A directed graph of task.
	typedef boost::adjacency_list<boost::vecS, boost::vecS,
		boost::directedS, Task::ID> Graph;
	/// Type of the vertices
	typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
	/// Vertex iterator
	typedef boost::graph_traits<Graph>::vertex_iterator VIterator;

	/// Add a task to the graph if it is not exist
	/// May throw CircularDependencyException
	/// \param [in] id The task id to be added.
	/// \return true if the task is added, false otherwise.
	void addTask(const Task& task);

	/// Delete a task from the graph
	/// May throw TaskNotFoundException
	/// \param [in] id The id of the to-be deleted task
	/// \return true if success, false otherwise.
	void deleteTask(const Task::ID id);

	/// Update a task from the graph
	/// This means rebuilding the graph since dependencies
	/// may change.
	/// May throw TaskNotFoundException if trying to update
	/// non existent task.
	/// \param [in] task The updated task
	void updateTask(const Task& task);

	/// Retrieve a single task from the graph
	/// May throw TaskNotFoundException
	/// \return The task with id \ref id
	Task getTask(const Task::ID id);

	/// Get the list of all tasks.
	/// \return Vector of all tasks.
	std::vector<Task> getTaskList() const;

	inline std::unordered_map<Task::ID, Task> getTaskTable() const {
		return taskTable;
	}

private:
	void rebuildGraph();

	Graph graph;
	std::unordered_map<Task::ID, Task> taskTable;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_H_

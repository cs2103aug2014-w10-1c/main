/// \file task_graph_controller.h
/// Defines the TaskGraphController class
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_CONTROLLER_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_CONTROLLER_H_

#include "../exception.h"
#include "../model.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Static utility class to manage a graph.
class TaskGraphController {
public:
	/// Add a task to the graph if it is not exist.
	/// May throw CircularDependencyException if it
	/// causes circular dependency.
	/// \param [in] task The task to be added.
	/// \return true if the task is added, false otherwise.
	static void addTask(TaskGraph& graph, const Task& task);

	/// Delete a task from the graph.
	/// May throw \ref Exception::TaskNotFoundException
	/// \param [in] id The id of the to-be deleted task
	/// \return true if success, false otherwise.
	static void deleteTask(TaskGraph& graph, const Task::ID id);

	/// Update a task from the graph
	/// Rebuilds the graph if there is a dependency change.
	/// May throw \ref Exception::TaskNotFoundException if trying
	/// to update non existent task.
	/// \param [in] task The updated task
	static void updateTask(TaskGraph& graph, const Task& task);

	/// Rebuild the graph based on task table.
	static void rebuildGraph(TaskGraph& graph);
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_CONTROLLER_H_

/// \file task_graph.h
/// Defines the TaskGraph class
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_H_

#include <vector>
#include "../api.h"

namespace You {
namespace QueryEngine {
namespace Internal {

/// Defines the task dependency graph
class TaskGraph {
public:
#if 0
	/// Add a task to the graph if it is not exist
	/// May throw TaskDoesNotExistException and
	/// CircularDependencyException
	/// \param [in] id The task id to be added.
	/// \return true if the task is added, false otherwise.
	void addTask(const Task& task);

	/// Delete a task from the graph
	/// May throw TaskDoesNotExistException
	/// \param [id] id The id of the to-be deleted task
	/// \return true if success, false otherwise.
	void deleteTask(const Task::ID id);

	/// Get the list of all tasks.
	/// \return Vector of all tasks.
	std::vector<Task> getTaskList() const;
private:
	/// The list of all tasks.
	std::vector<Task> allTasks;
#endif
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_TASK_GRAPH_H_

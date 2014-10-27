/// \file task_model.h
/// Defines the task model class.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_TASK_MODEL_H_
#define YOU_QUERYENGINE_TASK_MODEL_H_

#include <cstdint>
#include <string>
#include <unordered_set>
#include <vector>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace You {
namespace QueryEngine {
namespace UnitTests { class TaskBuilderTests; }
namespace Internal { class TaskSerializer; class TaskBuilder; }

/// Defines the task model
///
/// The instances of the task model can be created using
/// TaskBuilder. No-arg default constructor still
/// available to satisfy the requirement of TaskGraph \n
/// The task model has these following fields
///		- id
///		- description
///		- deadline
///		- priority
///		- dependencies
class Task {
	friend class Internal::TaskBuilder;
public:
	/// Constructor
	Task();

	/// \name Typedefs
	/// @{
	typedef int64_t ID;
	typedef std::wstring Description;
	typedef boost::posix_time::ptime Time;
	typedef std::unordered_set<Task::ID> Dependencies;
	enum class Priority { NORMAL, HIGH };
	typedef std::unordered_set<Task::ID> Subtasks;
	/// @}

	/// \name Inlined Field Getters.
	/// @{
	ID getID() const { return id; }
	Description getDescription() const { return description; }
	Time getDeadline() const { return deadline; }
	Dependencies getDependencies() const { return dependencies; }
	Priority getPriority() const { return priority; }
	bool isCompleted() const { return completed; }
	ID getParent() const { return parent; }
	Subtasks getSubtasks() const { return subtasks; }
	/// @}

	/// Check dependency
	/// \returns true if id is in the dependency, false otherwise.
	bool isDependOn(const Task::ID id) const;

	/// \name Field Setters
	/// @{
	void setDescription(const Description& description);
	void setDeadline(Time deadline);
	void setDependencies(const Dependencies& dependencies);
	void setPriority(Priority priority);
	void setCompleted(bool completed);
	void setParent(const Task::ID parent);
	void setSubtasks(const Subtasks& subtasks);
	/// @}

	/// \name Field Default Values
	/// @{
	static const ID DEFAULT_ID;
	static const Description DEFAULT_DESCRIPTION;
	static const Time NEVER;
	static const Time DEFAULT_DEADLINE;
	static const Dependencies DEFAULT_DEPENDENCIES;
	static const Priority DEFAULT_PRIORITY;
	static const Subtasks DEFAULT_SUBTASKS;
	/// @}

	/// Check equality of two tasks by comparing field by field.
	/// \param[in] rhs The task object to be compared
	inline bool operator==(const Task& rhs) const {
		return isStrictEqual(rhs);
	}

private:
	/// The all-field constructor called by the builder
	explicit Task(ID id, const Description& description, Time deadline,
		const Dependencies& dependencies, Priority priority, ID parent,
		Subtasks subtasks) :
		id(id), description(description), deadline(deadline),
		dependencies(dependencies), priority(priority), completed(false),
		parent(parent), subtasks(subtasks) {}

	/// Check if the task is strictly equal with another task
	/// Two taks are strictly equal if all fields are equal
	/// \returns The strict equality of two tasks.
	bool isStrictEqual(const Task& task) const;

	/// \name Private Fields
	/// @{
	ID id;
	Description description;
	Time deadline;
	Dependencies dependencies;
	Priority priority;
	bool completed;
	ID parent;
	Subtasks subtasks;
	/// @}
};

/// String representation of a task, for testing and logging.
///
/// \param[in] task The task object, assumed all fields are valid
/// \return A string representation of the task
std::wstring ToString(const Task& task);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TASK_MODEL_H_

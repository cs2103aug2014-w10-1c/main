/// \file task_model.h
/// Defines the task model class.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_TASK_MODEL_H_
#define YOU_QUERYENGINE_TASK_MODEL_H_

#include <cstdint>
#include <string>
#include <vector>

namespace You {
namespace QueryEngine {

// Do a forward declaration on TaskBuilder
// so that it can be
namespace Internal {
class TaskBuilder;
}

/// \brief The task model
/// The task instances must be created using builder pattern.
/// \see [Internal::TaskBuilder]
class Task {
	friend class Internal::TaskBuilder;
public:
	/// \name Typedefs
	/// @{
	typedef int64_t ID;
	typedef std::wstring Description;
	typedef std::int64_t Time;
	typedef std::vector<Task::ID> Dependencies;
	enum class Priority { IMPORTANT, NORMAL };
	/// @}

	/// \name Getters
	/// @{
	inline ID getID() const { return id; }
	inline Description getDescription() const { return description; }
	inline Time getDeadline() const { return deadline; }
	inline Dependencies getDependencies() const { return dependencies; }
	inline Priority getPriority() const { return priority; }
	/// @}

	/// \name Setters
	/// @{
	void setDescription(const Description&);
	void setDeadline(Time);
	void setDependencies(const Dependencies&);
	void setPriority(Priority);
	/// @}

	/// Return a task with using default values for all its fields.
	/// The ID of the task created is LAST_ID + 1
	static Task nextNewTask();

private:
	Task(ID id, const Description& description, Time deadline,
		const Dependencies& dependencies, Priority priority) :
		id(id), description(description), deadline(deadline),
		dependencies(dependencies), priority(priority) {}

	/// \name Private Fields
	/// @{
	ID id;
	Description description;
	Time deadline;
	Dependencies dependencies;
	Priority priority;
	/// @}

	/// \name Default Values
	/// @{
	static const Description DEFAULT_DESCRIPTION;
	static const Time DEFAULT_DEADLINE;
	static const Dependencies DEFAULT_DEPENDENCIES;
	static const Priority DEFAULT_PRIORITY;
	/// @}

	// TODO(evansb) Remove when getNextID() that talks to
	// Data Storage is implemented.
	static const ID DUMMY_ID;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TASK_MODEL_H_

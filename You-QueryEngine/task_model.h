/// \file task_model.h
/// Defines the task model class.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_TASK_MODEL_H_
#define YOU_QUERYENGINE_TASK_MODEL_H_

#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace You {
namespace QueryEngine {

// Forward declaration on TaskBuilder
namespace Internal {
class TaskBuilder;
}

/// \brief The task model
/// The task instances must be created using builder pattern.
/// \see [Internal::TaskBuilder]
class Task {
	friend class Internal::TaskBuilder;
public:
	/// \name Typedef for fields
	/// @{
	typedef int64_t ID;
	typedef std::wstring Description;
	typedef boost::posix_time::ptime Time;
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

	/// Check if the task is strictly equal with
	/// another task (Two taks are strictly equal if all fields
	/// are equal)
	bool isStrictEqual(const Task& task) const;

	/// \name Default Values
	/// @{
	static const ID DEFAULT_ID;
	static const Description DEFAULT_DESCRIPTION;
	static const Time NEVER;
	static const Time DEFAULT_DEADLINE;
	static const Dependencies DEFAULT_DEPENDENCIES;
	static const Priority DEFAULT_PRIORITY;
	/// @}

	/// Equality of task is determined by its unique ID
	/// For stricter equality, \see isStrictEqual
	inline bool operator==(const Task& rhs) const {
		return id == rhs.id;
	}

private:
	/// Disable default constructor - use builder only
	Task() = delete;
	/// Called by nextNewTask()
	explicit Task(ID id, const Description& description, Time deadline,
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
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TASK_MODEL_H_

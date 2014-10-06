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
#include <unordered_set>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/graph/adjacency_list.hpp>

namespace You {
namespace QueryEngine {
namespace UnitTests { class TaskBuilderTests; }
namespace Internal { class TaskBuilder; }

/// \brief The task model
/// The task instances must be created using builder pattern.
/// \see [Internal::TaskBuilder]
class Task {
	friend class Internal::TaskBuilder;
public:
	/// Constructor
	Task();

	/// \name Type Declarations for Task Fields
	/// @{
	typedef int64_t ID;
	typedef std::wstring Description;
	typedef boost::posix_time::ptime Time;
	typedef std::vector<Task::ID> Dependencies;
	enum class Priority { IMPORTANT, NORMAL };
	/// @}

	/// \name Getters for Field Values.
	/// @{
	inline ID getID() const { return id; }
	inline Description getDescription() const { return description; }
	inline Time getDeadline() const { return deadline; }
	inline Dependencies getDependencies() const { return dependencies; }
	inline Priority getPriority() const { return priority; }
	/// @}

	/// \name Setters for Field Values
	/// @{
	void setDescription(const Description&);
	void setDeadline(Time);
	void setDependencies(const Dependencies&);
	void setPriority(Priority);
	/// @}

	/// \name Default Values
	/// @{
	static const ID DEFAULT_ID;
	static const Description DEFAULT_DESCRIPTION;
	static const Time NEVER;
	static const Time DEFAULT_DEADLINE;
	static const Dependencies DEFAULT_DEPENDENCIES;
	static const Priority DEFAULT_PRIORITY;
	/// @}

	/// Check equality of two tasks by comparing field by field.
	/// \param[in] rhs The task object to be compared
	inline bool operator==(const Task& rhs) const {
		return isStrictEqual(rhs);
	}

	/// String representation of a task, for testing and logging.
	/// \param[in] The task object, assumed all fields are valid
	/// \return A string representation of the task
	static std::wstring ToString(const Task&);

private:
	/// Called by nextNewTask()
	explicit Task(ID id, const Description& description, Time deadline,
		const Dependencies& dependencies, Priority priority) :
		id(id), description(description), deadline(deadline),
		dependencies(dependencies), priority(priority) {}

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
	/// @}

	static const std::wstring FORMAT;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TASK_MODEL_H_

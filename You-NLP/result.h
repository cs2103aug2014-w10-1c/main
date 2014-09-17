#pragma once
#ifndef YOU_NLP_RESULT_H_
#define YOU_NLP_RESULT_H_

#include "task_list.h"

namespace You {
namespace NLP {

/// Defines the result of a natural language query.
class Result {
public:
	/// Checks if this result is a result set.
	///
	/// \return true if the result is a result set result.
	bool isResultSetResult() const;

	/// Checks if this result is a task object.
	///
	/// \return true if the result is an object result.
	bool isAffectedObjectResult() const;

	/// Checks if this result is a affected row count.
	///
	/// \return true if the result is an affected row count result.
	bool isAffectedCountResult() const;

private:
	/// The possible types of results.
	enum class Types {
		/// The result is a set of tasks.
		RESULT_SET,

		/// The result is the affected task.
		AFFECTED_OBJECT,

		/// The result is the number of affected tasks.
		AFFECTED_COUNT
	};

private:
	/// The type of this result.
	Types type;

	union {
		/// The task list, if the result is that.
		///
		/// \see TaskList
		/// \see type The type of the result
		TaskList taskList;

		/// The task, if the result is that.
		///
		/// \see Task
		/// \see type The type of the result
		Task task;

		/// The number of affected tasks, if the result is that.
		///
		/// \see type The type of the result
		size_t count;
	};
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_RESULT_H_

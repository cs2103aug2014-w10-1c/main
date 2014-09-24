#pragma once
#ifndef YOU_NLP_RESULT_H_
#define YOU_NLP_RESULT_H_

#include "task_list.h"

namespace You {
namespace NLP {

/// Defines the result of a natural language query.
class Result {
	friend class Controller;

public:
	/// Checks if this result is a result set.
	///
	/// \return true if the result is a result set result.
	bool isResultSetResult() const;

	/// Gets the result set if the current result is one.
	///
	/// \return The result set if this is a result set result. Behaviour is
	///         undefined otherwise.
	const TaskList& getResultSet() const;

	/// Checks if this result is a task object.
	///
	/// \return true if the result is an object result.
	bool isAffectedObjectResult() const;

	/// Gets the affected object if the current result is one.
	///
	/// \return The affected object if this is an affected object result.
	///         Behaviour is undefined otherwise.
	const Task& getAffectedObject() const;

	/// Checks if this result is a affected row count.
	///
	/// \return true if the result is an affected row count result.
	bool isAffectedCountResult() const;

	/// Gets the affected object count if the current result is one.
	///
	/// \return The number of affected objects if this is an affected object
	///         count result. Behaviour is undefined otherwise.
	const size_t getAffectedCount() const;

private:
	/// The union type of all possible results we can return.
	typedef boost::variant<TaskList, Task, size_t> ResultType;

private:
	/// Constructor.
	///
	/// \param[in] result The result from the query engine.
	explicit Result(const ResultType& result);

	/// Move constructor.
	///
	/// \param[in] result The result from the query engine.
	Result(ResultType&& result);

private:
	/// The actual result type.
	ResultType record;
};

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_RESULT_H_

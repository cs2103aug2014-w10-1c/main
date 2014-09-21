#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_

#include "task_model.h"

namespace You {
namespace QueryEngine {

/// \class Task::Builder
/// Builder for tasks, new instance of task should be created using this
/// builder.
///
/// Example usage:
///   \code{.cpp}
///	    Task newTask = Task::Builder::get().description("Feed");
///   \endcode
class Task::Builder {
	friend class Task;
public:
	/// Return a new builder instance.
	static Builder get();

	Builder& description(const Task::Description& description);
	Builder& deadline(Task::Time deadline);
	Builder& dependencies(const Task::Dependencies& dependencies);
	Builder& priority(Task::Priority priority);

	~Builder() = default;
private:
	Builder();
	explicit Builder(const Task& instance) : instance(instance) { }

	// Instance that will be returned by the builder
	Task instance;
};

}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_TASK_BUILDER_H_

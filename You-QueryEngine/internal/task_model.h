#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_MODEL_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_MODEL_H_

#include "../stdafx.h"
#include "exception.h"

namespace You {
namespace QueryEngine {

/// \class Task
/// \brief The task model
/// The task instances are created using builder pattern.
class Task {
public:
	// Type definitions of each fields.
	typedef int64_t ID;
	typedef std::wstring Description;
	typedef std::int64_t Time;
	typedef std::vector<Task::ID> Dependencies;
	enum class Priority { IMPORTANT, NORMAL };

	// Field getters
	inline ID getID() const { return id; }
	inline Description getDescription() const { return description; }
	inline Time getDeadline() const { return deadline; }
	inline Dependencies getDependencies() const { return dependencies; }
	inline Priority getPriority() const { return priority; }

	// Field setters
	void setDescription(const Description&);
	void setDeadline(Time);
	void setDependencies(const Dependencies&);
	void setPriority(Priority);

public:
	/// \class Builder
	/// Singleton builder class, new instance of task should be created using this
	/// builder.
	///
	/// \remarks Note that the temporary instance variable will be resetted
	/// each time build is invoked.
	///
	/// Example usage:
	///   \code{.cpp}
	///	    Task newTask = Task::Builder::id(2000).description("Feed the kitten");
	///   \endcode
	class Builder {
		friend class Task;
	public:
		static Builder&& description(const Description &description);
		static Builder&& deadline(Time deadline);
		static Builder&& dependencies(const Dependencies &dependencies);
		static Builder&& priority(Priority priority);

		// Cannot copy or assign singletons.
		Builder(Builder&) = delete;
		Builder& operator=(const Builder&) = delete;

	private:
		/// Fetch new task ID from Data Storage.
		static Builder&& id(ID id);

		/// Get instance of the singleton builder.
		static Builder&& get();

		Builder() = default;
		// Instance returned by the builder
		static Task instance;
	};  // class Builder

	Task(const Task::Builder &&builder); // NOLINT
	Task&& operator=(const Task::Builder &&builder);

private:
	Task(ID id, const Description& description, Time deadline,
		const Dependencies& dependencies, Priority priority) :
		id(id), description(description), deadline(deadline),
		dependencies(dependencies), priority(priority) {}

	/// Return a task with using default values for all its fields.
	/// The ID of the task created is LAST_ID + 1
	static Task nextNewTask();

	// Fields
	ID id;
	Description description;
	Time deadline;
	Dependencies dependencies;
	Priority priority;

	// Default values for fields
	static const Description DEFAULT_DESCRIPTION;
	static const Time DEFAULT_DEADLINE;
	static const Dependencies DEFAULT_DEPENDENCIES;
	static const Priority DEFAULT_PRIORITY;

	// TODO(evansb) Remove when getNextID() that talks to
	// Data Storage is implemented.
	static const ID DUMMY_ID;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_TASK_MODEL_H_

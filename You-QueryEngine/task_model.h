#pragma once
#ifndef YOU_QUERYENGINE_TASK_MODEL_H_
#define YOU_QUERYENGINE_TASK_MODEL_H_

#include <string>
#include <vector>
#include <cstdint>

namespace You {
namespace QueryEngine {

/// Class for task objects.
/// The task instances are created using builder pattern.
class Task {
	/// Type definitions of each fields.
	typedef int64_t ID;
	typedef std::wstring Description;
	typedef std::int64_t Time;
	typedef std::vector<Task> Dependencies;
	enum class Priority { IMPORTANT, NORMAL };

private:
	const ID id;
	Description description;
	Time deadline;
	Dependencies dependencies;
	Priority priority;

public:
	// Field getters
	inline ID getID() const { return id; }
	inline Description getDescription() const { return description; }
	inline Time getDeadline() const { return deadline; }
	inline Dependencies getDependencies() const { return dependencies; }
	inline Priority getPriority() const { return priority; }

public:
	// Field setters
	void setID(ID);
	void setDescription(const Description&);
	void setDeadline(Time);
	void setDependencies(const Dependencies&);
	void setPriority(Priority);

private:
	// Default values for fields
	static const Time DEFAULT_TIME;
	static const Description DEFAULT_DESCRIPTION;
	static const Dependencies DEFAULT_DEPENDENCIES;
	static const Priority DEFAULT_PRIORITY;

public:
	// Singleton builder class, new instance of task should be
	// created from here.
	class Builder {
	public:
		static Builder& get();
		static Builder&	id(ID id);
		static Builder& description(const Description& description);
		static Builder& deadline(Time deadline);
		static Builder& dependencies(const Dependencies& dependencies);
		static Builder& priority(Priority priority);
		static Task build();

	public:
		Builder(Builder&) = delete;
		Builder& operator=(const Builder&) = delete;

	private:
		Builder() = default;
	};  // class Builder

private:
	Task() = default;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TASK_MODEL_H_

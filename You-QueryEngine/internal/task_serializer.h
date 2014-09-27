/// \file task_builder.h
/// Defines the task serializer class
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_

#include <string>
#include <unordered_map>
#include "../task_model.h"

namespace You {
namespace QueryEngine {
namespace Internal {

///   Utility class to serialize and deserialize tasks.
///
///   Example usage:
///   \code{.cpp}
///     auto fetched = DataStorage.get(id);
///	    Task newTask = TaskSerializer::deserialize(fetched);
///   \endcode
class TaskSerializer {
public:
	typedef std::unordered_map<std::wstring, std::wstring> STask;
	/// Serialize a task model
	/// \return Serialized form of the task .
	static STask serialize(const Task&);

	/// Deserialize a serialized form of task model
	/// \return The deserialized task.
	static Task deserialize(const STask&);

private:
	/// Disable constructor, this is a utility class
	TaskBuilder();
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_

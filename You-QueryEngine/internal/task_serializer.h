/// \file task_serializer.h
/// Defines the task serializer class
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_

#include <string>
#include <unordered_map>
#include <boost/format.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
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
	/// \name Typedefs
	/// @{
	typedef std::wstring Key;
	typedef std::wstring Value;
	typedef std::unordered_map<Key, Value> STask;
	/// @}

	/// Serialize a task model
	/// \return Serialized form of the task .
	static STask serialize(const Task&);

	/// Deserialize a serialized form of task model
	/// \return The deserialized task.
	static Task deserialize(const STask&);

	/// \name Serialized task field names.
	/// @{
	static const Key KEY_DESCRIPTION;
	static const Key KEY_DEADLINE;
	static const Key KEY_PRIORITY;
	static const Key KEY_DEPENDENCIES;
	/// @}

	/// \name String for priorities
	/// @{
	static const Value VALUE_PRIORITY_NORMAL;
	static const Value VALUE_PRIORITY_IMPORTANT;
	/// @}

	/// Delimiter for dependencies and deadlines
	static const Value VALUE_DELIMITER;

private:
	/// \name Serializer for each fields.
	/// @{
	/// The ID converted to std::wstring using lexical_cast
	static Value serializeID(Task::ID id);
	/// Description is already the same
	static Value serializeDescription(Task::Description description);
	/// YYYY:MM:DD HH::MM::SS to "YYYY;MM;DD;HH;MM;SS;"
	static Value serializeDeadline(Task::Time deadline);
	/// Lowercased enum name
	static Value serializePriority(Task::Priority priority);
	/// { 1,2,3 } to "1;2;3"
	static Value serializeDependencies(Task::Dependencies dependencies);
	/// @}

	/// Disable constructor, this is a utility class
	TaskSerializer();
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_

/// \file task_serializer.h
/// Defines the task serializer class
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_
#define YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_

#include <string>
#include <unordered_map>
#include <boost/tokenizer.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "../task_model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class TaskSerializerTests;  }
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
	static const Key KEY_ID;
	static const Key KEY_DESCRIPTION;
	static const Key KEY_DEADLINE;
	static const Key KEY_PRIORITY;
	static const Key KEY_DEPENDENCIES;
	/// @}

	/// \name String for priorities
	/// @{
	static const Value VALUE_PRIORITY_NORMAL;
	static const Value VALUE_PRIORITY_HIGH;
	/// @}

	/// Delimiter for dependencies and deadlines
	static const Value VALUE_DELIMITER;

private:
	/// Disable constructor, this is a utility class
	TaskSerializer();

	/// \name Serializer for each fields.
	/// @{
	/// The ID converted to std::wstring using lexical_cast
	static Value serializeID(Task::ID id);
	/// Description is already the same
	static Value serializeDescription(const Task::Description& description);
	/// YYYY:MM:DD HH::MM::SS to "YYYY;MM;DD;HH;MM;SS;"
	static Value serializeDeadline(const Task::Time& deadline);
	/// Lowercased enum name
	static Value serializePriority(const Task::Priority& priority);
	/// { 1,2,3 } to "1;2;3"
	static Value serializeDependencies(const Task::Dependencies& dependencies);
	/// @}

	/// \name Deserializer for each fields.
	/// @{
	/// Lexical_cast back the ID to int64
	static Task::ID deserializeID(const Value& id);
	/// Description is already the same
	static Task::Description deserializeDescription(const Value& description);
	/// "YYYY;MM;DD;HH;MM;SS;" to ptime object
	static Task::Time deserializeDeadline(const Value& deadline);
	/// Lowercased enum name to enum
	static Task::Priority deserializePriority(const Value& priority);
	/// "1;2;3" to { 1,2,3 }
	static Task::Dependencies deserializeDependencies(const Value& dependencies);
	/// @}

	/// Tokenize a string by VALUE_DELIMITER
	static std::vector<std::wstring> tokenize(const Value& input);

	/// Maps string to priority
	static const std::unordered_map<Value, Task::Priority> strPrioTable;
	/// Maps priority to string
	static const std::unordered_map<Task::Priority, Value> prioStrTable;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_TASK_SERIALIZER_H_

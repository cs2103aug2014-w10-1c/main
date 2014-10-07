#pragma once
#ifndef YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_
#define YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_

#include <cstdint>
#include <functional>
#include <unordered_map>
#include "boost/lexical_cast.hpp"

#define PUGIXML_WCHAR_MODE
#include "pugixml.hpp"

#include "../task_typedefs.h"

namespace You {
namespace DataStore {
namespace UnitTests { class InternalDataStoreTest; }

/// The internal components of DataStore
namespace Internal {

/// The most primitive class that does the changes to the actual xml file
class InternalDataStore {
	/// Test classes
	friend class You::DataStore::UnitTests::InternalDataStoreTest;
public:
	/// Insert a task into the datastore
	/// \return true if insertion successful,
	/// \return false if task id already exists
	bool post(TaskId, const SerializedTask&);

	/// Update the content of a task
	/// \return true if update successful,
	/// \return false if task id specified does not exist
	bool put(TaskId, const SerializedTask&);

	/// Delete a task
	/// \return true if erase successful,
	/// \return false if task id specified does not exist
	bool erase(TaskId);

	/// Get a task
	/// \return false and an empty SerializedTask if task id does not exist
	SerializedTask getTask(TaskId);

	/// Get all tasks
	/// \return a vector of all tasks in unordered map form
	std::vector<SerializedTask> getAllTask();

	/// Saves the xml object to a file
	/// \return true if operation successful and false otherwise
	bool saveData();

	/// Loads the xml file into the document variable
	void loadData();

private:
	static const std::wstring FILE_PATH;
	pugi::xml_document document;

	/// Serialize task to an xml node
	void serialize(const SerializedTask&, pugi::xml_node&);

	/// Deserialize task from an xml node
	SerializedTask deserialize(const pugi::xml_node&);
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_

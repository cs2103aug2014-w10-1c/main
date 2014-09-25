#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <cstdint>
#include <functional>
#include <unordered_map>

#include "pugixml.hpp"

namespace You {
namespace DataStore {

class DataStore {
public:
	/// Test classes
	friend class DataStoreAPITest;

	/// typedefs
	typedef std::unordered_map<std::wstring, std::wstring> Task;
	typedef int64_t TaskId;

	DataStore() = default;
	~DataStore() = default;

	/// Insert a task into the datastore
	void post(Task);

	/// Update the content of a task
	void put(TaskId, Task);

	/// Get a task
	Task get(TaskId);

	/// Delete a task
	void erase(TaskId);

	/// Commits the changes
	void commit();

	/// Roll back to the last commited changes
	void rollback();

	/// Get a list of tasks that passes the filter
	std::vector<Task> filter(const std::function<bool(Task)>&);

private:
	static const std::wstring FILE_PATH;
	pugi::xml_document document;

	/// Saves the xml object to a file
	/// Returns true if operation successful and false otherwise
	bool saveData();
	/// Loads the xml file into the xml object
	void loadData();
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_DATASTORE_H_

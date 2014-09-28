#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <cstdint>
#include <functional>
#include <unordered_map>
#include "boost/variant.hpp"

#include "pugixml.hpp"

namespace You {
namespace DataStore {

class DataStore {
	/// Test classes
	friend class DataStoreAPITest;
public:
	/// typedefs
	/// typedef for Serialized Task
	typedef std::unordered_map<std::wstring, std::wstring> STask;
	/// typedef for Task ID
	typedef int64_t TaskId;

	/// Insert a task into the datastore
	///
	/// Returns true if insertion successful,
	/// false if task id already exists
	bool post(TaskId, const STask&);

	/// Update the content of a task
	///
	/// Returns true if update successful,
	/// false if task id specified does not exist
	bool put(TaskId, const STask&);

	/// Get a task
	///
	/// Returns false and an empty STask if task id does not exist
	boost::variant<bool, STask> get(TaskId);

	/// Delete a task
	///
	/// Returns true if erase successful,
	/// false if task id specified does not exist
	bool erase(TaskId);

	/// Get a list of tasks that passes the filter
	std::vector<STask> filter(const std::function<bool(STask)>&);

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

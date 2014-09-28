#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <cstdint>
#include <functional>
#include <unordered_map>
#include "boost/variant.hpp"
#include "boost/lexical_cast.hpp"

#include "pugixml.hpp"

namespace You {
namespace DataStore {

class DataStore {
	/// Test classes
	friend class DataStoreAPITest;
public:
	/// \name typedefs
	/// typedef for Task ID
	typedef int64_t TaskId;
	/// Typedefs for serializing to xml
	typedef std::wstring Key;
	typedef std::wstring Value;
	typedef std::pair<Key, Value> KeyValuePair;
	typedef std::unordered_map<Key, Value> STask;


	/// Insert a task into the datastore
	/// \return true if insertion successful,
	/// \return false if task id already exists
	bool post(TaskId, const STask&);

	/// Update the content of a task
	/// \return true if update successful,
	/// \return false if task id specified does not exist
	bool put(TaskId, const STask&);

	/// Get a task
	/// \return false and an empty STask if task id does not exist
	boost::variant<bool, STask> get(TaskId);

	/// Delete a task
	/// \return true if erase successful,
	/// \return false if task id specified does not exist
	bool erase(TaskId);

	/// Get a list of tasks that passes the filter
	std::vector<STask> filter(const std::function<bool(STask)>&);

private:
	static const std::wstring FILE_PATH;
	pugi::xml_document document;

	/// Saves the xml object to a file
	/// \return true if operation successful and false otherwise
	bool saveData();
	/// Loads the xml file into the xml object
	void loadData();
	/// Serialize task to an xml node
	void serialize(const STask&, pugi::xml_node*);
	/// Deserialize task from an xml node
	STask deserialize(const pugi::xml_node&);
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_DATASTORE_H_

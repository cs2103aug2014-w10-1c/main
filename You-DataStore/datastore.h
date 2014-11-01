//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <vector>
#include "task_typedefs.h"
#include "transaction.h"

namespace You {
namespace DataStore {

/// The public methods of the data store.
///
/// This is actually a facade for the \ref Internal::DataStore class.
class DataStore {
public:
	/// Get the instance of DataStore
	static DataStore& get();

	/// Start a new transaction
	/// \return A reference to a new \ref Transaction
	Transaction begin();

	/// Methods modifying tasks
	/// @{
	/// Push a post operation to operation queue
	void post(TaskId, const KeyValuePairs&);
	/// Push a put operation to operation queue
	void put(TaskId, const KeyValuePairs&);
	/// Push an erase operation to operation queue
	void erase(TaskId);
	/// @}

	/// Methods modifying application resources
	/// @{
	/// Push a post operation to operation queue
	void post(std::wstring, const KeyValuePairs&);
	/// Push a put operation to operation queue
	void put(std::wstring, const KeyValuePairs&);
	/// Push an erase operation to operation queue
	void erase(std::wstring);

	/// Get all tasks
	/// \return a vector of KeyValuePairs
	std::vector<KeyValuePairs> getAllTasks();

	/// Get all resources
	/// \return a vector of KeyValuePairs
	std::vector<KeyValuePairs> getAllResources();

	/// Wipe the datastore
	void wipeData();

private:
	/// Default constructor. Use \ref get to get the singleton instance.
	DataStore() = default;
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_DATASTORE_H_

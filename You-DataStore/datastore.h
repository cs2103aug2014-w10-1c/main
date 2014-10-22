#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <vector>
#include "task_typedefs.h"
#include "transaction.h"

namespace You {
namespace DataStore {
namespace UnitTests { class DataStoreApiTest; }

/// The public methods of the data store.
///
/// This is actually a facade for the \ref Internal::DataStore class.
class DataStore {
	friend class UnitTests::DataStoreApiTest;

public:
	/// Get the instance of DataStore
	static DataStore& get();

	/// Start a new transaction
	/// \return A reference to a new \ref Transaction
	Transaction begin();

	/// Modifying methods
	/// @{
	/// Push a post operation to operation queue
	void post(TaskId, const KeyValuePairs&);
	/// Push a put operation to operation queue
	void put(TaskId, const KeyValuePairs&);
	/// Push an erase operation to operation queue
	void erase(TaskId);
	/// @}

	/// Get all tasks
	/// \return a vector of SerializedTasks
	std::vector<KeyValuePairs> getAllTasks();

private:
	/// Default constructor. Use \ref get to get the singleton instance.
	DataStore() = default;
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_DATASTORE_H_

#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <stack>
#include <memory>
#include "task_typedefs.h"
#include "transaction.h"

namespace You {
namespace DataStore {
namespace UnitTests { class DataStoreApiTest; }
namespace Internal { class InternalDataStore; }

class DataStore {
	friend class UnitTests::DataStoreApiTest;
public:
	DataStore();

	/// Start a new transaction
	/// It will do a busy wait if there is another transaction taking place
	/// \return a reference to a new Transaction
	Transaction& begin();

	/// Get the instance of DataStore
	static DataStore& get();

	// Modifying methods
	/// Push a post operation to operation queue
	void post(TaskId, const SerializedTask&);
	/// Push a put operation to operation queue
	void put(TaskId, const SerializedTask&);
	/// Push an erase operation to operation queue
	void erase(TaskId);

	/// Get all tasks
	/// \return a vector of SerializedTasks
	std::vector<SerializedTask> getAllTask();

	/// Notify DataStore that a transaction is committed
	/// To be used only by Transaction object
	void notifyCommit();

	/// Notify DataStore that a transaction is rolled back
	/// To be used only by Transaction object
	void notifyRollback();

	/// Get the instance of InternalDataStore
	Internal::InternalDataStore& getInternal();

private:
	/// Boolean to ensure mutual exclusion
	bool isServing = false;

	std::stack<std::shared_ptr<Transaction>> transactionStack;

	std::unique_ptr<Internal::InternalDataStore> internalDataStore;
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_DATASTORE_H_

#pragma once
#ifndef YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_
#define YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_

#include <stack>
#include <memory>

#include "pugixml.h"

#include "internal_transaction.h"
#include "../transaction.h"
#include "../task_typedefs.h"

namespace You {
namespace DataStore {
namespace UnitTests { class DataStoreTest; }

/// The internal components of DataStore
namespace Internal {

/// The most primitive class that does the changes to the actual xml file
class DataStore {
	/// Test classes
	friend class You::DataStore::UnitTests::DataStoreTest;
	friend class You::DataStore::UnitTests::DataStoreApiTest;
public:
	/// Gets the singleton instance of the internal data store.
	///
	/// \return The internal data store instance.
	static DataStore& get();

	/// Transaction management.
	/// @{

	/// Starts a new transaction.
	///
	/// \return A reference to a new \ref DataStrore::Transaction
	You::DataStore::Transaction begin();

	/// Notifies the data store that the given transaction is being committed.
	///
	/// \param[in] transaction The transaction being committed.
	void onTransactionCommit(Transaction& transaction);

	/// Notifies the data store that the given transaction is being rolled back.
	///
	/// \param[in] transaction The transaction being rolled back.
	void onTransactionRollback(Transaction& transaction);

	/// @}

	/// Insert a task into the datastore
	void post(TaskId, const SerializedTask&);

	/// Update the content of a task
	void put(TaskId, const SerializedTask&);

	/// Delete a task
	void erase(TaskId);

	/// Get all tasks
	/// \return a vector of all tasks in unordered map form
	std::vector<SerializedTask> getAllTask();

private:
	DataStore() = default;

	/// Saves the xml object to a file
	/// \return true if operation successful and false otherwise
	bool saveData();

	/// Loads the xml file into the document variable
	void loadData();

	/// Executes the operation queue into the xml_document
	///
	/// \param[in] opQueue operations queue to be executed
	/// \param[in] xml xml document to be modified by the operations
	void executeTransaction(Transaction& transaction, pugi::xml_document& xml);

private:
	static const std::wstring FILE_PATH;
	pugi::xml_document document;

	/// The current stack of active transactions.
	std::stack<std::weak_ptr<Transaction>> transactionStack;

	/// The stack of committed transactions.
	std::stack<std::weak_ptr<Transaction>> committedTransaction;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_

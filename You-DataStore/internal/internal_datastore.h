#pragma once
#ifndef YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_
#define YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_

#include <stack>
#include <memory>

#define PUGIXML_WCHAR_MODE
#include "pugixml.hpp"

#include "internal_transaction.h"
#include "../transaction.h"
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
	/// Gets the singleton instance of the internal data store.
	///
	/// \return The internal data store instance.
	static InternalDataStore& get();

	/// Transaction management.
	/// @{

	/// Starts a new transaction.
	///
	/// \return A reference to a new \ref DataStrore::Transaction
	You::DataStore::Transaction begin();

	/// Notifies the data store that the given transaction is being committed.
	///
	/// \param[in] transaction The transaction being committed.
	void onTransactionCommit(const Transaction& transaction);

	/// Notifies the data store that the given transaction is being rolled back.
	///
	/// \param[in] transaction The transaction being rolled back.
	void onTransactionRollBack(const Transaction& transaction);

	/// @}

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
	InternalDataStore() = default;

private:
	static const std::wstring FILE_PATH;
	pugi::xml_document document;

	/// Serialize task to an xml node
	void serialize(const SerializedTask&, pugi::xml_node&);

	/// Deserialize task from an xml node
	SerializedTask deserialize(const pugi::xml_node&);

private:
	/// The current stack of transactions active.
	std::stack<std::weak_ptr<Transaction>> transactionStack;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_

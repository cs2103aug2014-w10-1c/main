//@author A0114171W
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
namespace UnitTests {
class DataStoreTest;
class DataStoreApiTest;
}

/// The internal components of DataStore
namespace Internal {

/// The most primitive class that does the changes to the actual xml file
class DataStore {
	/// Test classes
	friend class UnitTests::DataStoreTest;
	friend class UnitTests::DataStoreApiTest;
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
	/// \param[out] transaction The transaction being committed.
	void onTransactionCommit(Transaction& transaction);

	/// Notifies the data store that the given transaction is being rolled back.
	///
	/// \param[out] transaction The transaction being rolled back.
	void onTransactionRollback(Transaction& transaction);

	/// @}

	/// Pushes \ref PostOperation into the active \ref Transaction
	void post(std::wstring branch, std::wstring id, const KeyValuePairs& kvp);

	/// Pushes \ref PutOperation into the active \ref Transaction
	void put(std::wstring branch, std::wstring id, const KeyValuePairs& kvp);

	/// Pushes \ref EraseOperation into the active \ref Transaction
	void erase(std::wstring branch, std::wstring id);

	/// General all getter of a data from the direct child of
	/// the root node of the XML document
	///
	/// \param[in] nodeName the name of the child
	/// \return a vector of unordered map of data in the xml node
	/// with the specified nodeName
	std::vector<KeyValuePairs> getAll(std::wstring nodeName);

	/// Wipe the datastore
	void wipeData();

private:
	DataStore() = default;

	/// Saves the xml object to a file
	/// \return true if operation successful and false otherwise
	bool saveData();

	/// Loads the xml file into the document variable
	/// May throw \ref DataStoreCorruptException when parsing fails
	/// \exception NotWellFormedException The exception thrown if data.xml is
	///									  not well formed
	/// \exception IOException The exception thrown if I/O error occurs
	void loadData();

	/// Executes the operation queue into the xml_document
	///
	/// \param[in] transaction The transaction to be executed
	/// \param[out] xml The xml document to be modified by the operations
	void executeTransaction(Transaction& transaction, pugi::xml_node& node);

	/// Throws the exception for the xml_parse_status given
	///
	/// \param[in] result The xml_parse_result
	void onXmlParseResult(const pugi::xml_parse_result& result);

private:
	static const std::string FILE_PATH;
	static const std::wstring ROOT_NODE_NAME;

	/// The xml_document representation of data.xml
	pugi::xml_document document;

	/// The root node of data.xml
	pugi::xml_node root;

	/// The current stack of active transactions.
	std::stack<std::weak_ptr<Transaction>> transactionStack;

	/// The log category for \ref Internal::DataStore
	static const std::wstring LOG_CATEGORY;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_INTERNAL_DATASTORE_H_

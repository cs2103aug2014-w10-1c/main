//@author A0114171W
#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "operation.h"
#include "operations/post_operation.h"
#include "operations/put_operation.h"
#include "operations/erase_operation.h"
#include "operations/branch_operation.h"
#include "internal_transaction.h"
#include "internal_datastore.h"

namespace You {
namespace DataStore {
namespace Internal {

const std::string DataStore::FILE_PATH = std::string("data.xml");

DataStore& DataStore::get() {
	static DataStore store;
	return store;
}

You::DataStore::Transaction DataStore::begin() {
	You::DataStore::Transaction result;
	transactionStack.push(std::weak_ptr<Internal::Transaction>(result));

	return result;
}

void DataStore::onTransactionCommit(Transaction& transaction) {
	// Only transaction on top of the stack may be committed
	assert(*transactionStack.top().lock() == transaction);

	auto self = transactionStack.top();
	transactionStack.pop();

	if (transactionStack.empty()) {
		// it is the only active transaction, execute the operations and save
		pugi::xml_document temp;
		temp.reset(document);
		executeTransaction(transaction, temp);
		document.reset(temp);
		saveData();
	} else {
		// There is a transaction before it that is yet to be committed.
		// Merge with that transaction
		auto below = transactionStack.top().lock();
		below->mergeOperationsQueue(transaction.operationsQueue);
		below->mergeOperationsQueue(transaction.mergedOperationsQueue);
	}
}

void DataStore::onTransactionRollback(Transaction& transaction) {
	// Can only rollback the latest transaction
	assert(*(transactionStack.top().lock()) == transaction);
	transactionStack.pop();
}

void DataStore::post(std::wstring branch, std::wstring id,
	const KeyValuePairs& kvp) {
	assert(!transactionStack.empty());

	std::unique_ptr<Internal::Operation> operation =
		std::make_unique<Internal::PostOperation>(branch, id, kvp);

	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
}

void DataStore::put(std::wstring branch, std::wstring id,
	const KeyValuePairs& kvp) {
	assert(!transactionStack.empty());

	std::unique_ptr<Internal::Operation> operation =
		std::make_unique<Internal::PutOperation>(branch, id, kvp);

	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
}

void DataStore::erase(std::wstring branch, std::wstring id) {
	assert(!transactionStack.empty());

	std::unique_ptr<Internal::Operation> operation =
		std::make_unique<Internal::EraseOperation>(branch, id);

	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
}

std::vector<KeyValuePairs> DataStore::getAll(std::wstring nodeName) {
	loadData();
	pugi::xml_node dataNode = BranchOperation::get(document, nodeName);
	std::vector<KeyValuePairs> allData;
	for (auto i = dataNode.begin(); i != dataNode.end(); ++i) {
		allData.push_back(SerializationOperation::deserialize(*i));
	}
	return allData;
}

void DataStore::wipeData() {
	document.reset();
	std::remove(FILE_PATH.c_str());
}

bool DataStore::saveData() {
	bool status = document.save_file(FILE_PATH.c_str());
	return status;
}

void DataStore::loadData() {
	bool isInitialized = !document.first_child().empty();
	if (!isInitialized) {
		pugi::xml_parse_result status = document.load_file(FILE_PATH.c_str());
	}
}

void DataStore::executeTransaction(Transaction & transaction,
	pugi::xml_document& xml) {
	for (auto operation = transaction.operationsQueue.begin();
		operation != transaction.operationsQueue.end();
		++operation) {
		bool status = operation->run(xml);
		assert(status);
		if (!status) {
			// throw exception/assert
		}
	}
	for (auto mergedOperation = transaction.mergedOperationsQueue.begin();
		mergedOperation != transaction.mergedOperationsQueue.end();
		++mergedOperation) {
		bool status = mergedOperation->run(xml);
		assert(status);
		if (!status) {
			// throw exception/assert
		}
	}
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

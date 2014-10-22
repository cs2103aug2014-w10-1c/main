#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "operation.h"
#include "operations/post_operation.h"
#include "operations/put_operation.h"
#include "operations/erase_operation.h"
#include "internal_transaction.h"
#include "internal_datastore.h"

namespace You {
namespace DataStore {
namespace Internal {

const std::wstring DataStore::FILE_PATH = std::wstring(L"data.xml");
const std::wstring DataStore::TASKS_NODE = std::wstring(L"tasks");
const std::wstring DataStore::DATA_NODE = std::wstring(L"data");

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
	// TODO(digawp): commit not the latest transaction
	// assume only transaction on top of the stack may be committed
	assert(*transactionStack.top().lock() == transaction);

	auto self = transactionStack.top();
	transactionStack.pop();

	if (transactionStack.empty()) {
		// it is the only active transaction, execute the operations and save
		pugi::xml_document temp;
		temp.reset(document);
		pugi::xml_node tasksNode = getTasksNode();
		executeTransaction(transaction, tasksNode);
		document.reset(temp);
		committedTransaction.push(self);
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
	// TODO(digawp): rollback not the latest transaction
	assert(*(transactionStack.top().lock()) == transaction);
	transactionStack.pop();
}

void DataStore::post(TaskId rawId, const KeyValuePairs& sTask) {
	assert(!transactionStack.empty());

	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::PostOperation>(rawId, sTask);

	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
}

void DataStore::put(TaskId rawId, const KeyValuePairs& sTask) {
	assert(!transactionStack.empty());

	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::PutOperation>(rawId, sTask);

	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
}

void DataStore::erase(TaskId rawId) {
	assert(!transactionStack.empty());

	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::EraseOperation>(rawId);

	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
}

std::vector<KeyValuePairs> DataStore::getAllTask() {
	loadData();
	pugi::xml_node tasksNode = getTasksNode();
	std::vector<KeyValuePairs> allTask;
	for (auto i = tasksNode.begin(); i != tasksNode.end(); ++i) {
		allTask.push_back(SerializationOperation::deserialize(*i));
	}
	return allTask;
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
	pugi::xml_node& xml) {
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

pugi::xml_node DataStore::getTasksNode() {
	pugi::xml_node tasksNode = document.child(TASKS_NODE.c_str());
	if (tasksNode.empty()) {
		tasksNode = document.append_child(TASKS_NODE.c_str());
		return tasksNode;
	}
	return tasksNode;
}

pugi::xml_node DataStore::getDataNode() {
	pugi::xml_node dataNode = document.child(DATA_NODE.c_str());
	if (dataNode.empty()) {
		dataNode = document.append_child(DATA_NODE.c_str());
		return dataNode;
	}
	return dataNode;
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

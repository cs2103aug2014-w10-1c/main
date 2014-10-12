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
	assert(*transactionStack.top().lock() == transaction);

	auto self = transactionStack.top();
	transactionStack.pop();

	if (transactionStack.empty()) {
		pugi::xml_document temp;
		temp.reset(document);
		for (auto operation = transaction.operationsQueue.begin();
			operation != transaction.operationsQueue.end();
			++operation) {
			bool status = !operation->run(temp);
			assert(!status);
			if (!status) {
				// throw exception/assert
			}
		}
		for (auto mergedOperation = transaction.mergedOperationsQueue.begin();
			mergedOperation != transaction.mergedOperationsQueue.end();
			++mergedOperation) {
			bool status = !mergedOperation->run(temp);
			assert(!status);
			if (!status) {
				// throw exception/assert
			}
		}
		document.reset(temp);
		committedTransaction.push(self);
	} else {
		auto below = transactionStack.top().lock();

		below->mergeQueue(transaction.operationsQueue);
		below->mergeQueue(transaction.mergedOperationsQueue);
	}
}

void DataStore::onTransactionRollback(Transaction& transaction) {
	transaction.operationsQueue.clear();
}

bool DataStore::post(TaskId rawId, const SerializedTask& sTask) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::PostOperation>(rawId, sTask);

	if (!transactionStack.empty()) {
		if (auto transaction = transactionStack.top().lock()) {
			transaction->push(std::move(operation));
			return true;
		}
	}

	return operation->run(document);
}

bool DataStore::put(TaskId rawId, const SerializedTask& sTask) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::PutOperation>(rawId, sTask);

	if (!transactionStack.empty()) {
		if (auto transaction = transactionStack.top().lock()) {
			transaction->push(std::move(operation));
			return true;
		}
	}

	return operation->run(document);
}

bool DataStore::erase(TaskId rawId) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::EraseOperation>(rawId);
	if (!transactionStack.empty()) {
		if (auto transaction = transactionStack.top().lock()) {
			transaction->push(std::move(operation));
			return true;
		}
	}

	return operation->run(document);
}

SerializedTask DataStore::getTask(TaskId rawId) {
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	pugi::xml_node toGet =
		document.find_child_by_attribute(L"id", taskId.c_str());

	return SerializationOperation::deserialize(toGet);
}

std::vector<SerializedTask> DataStore::getAllTask() {
	loadData();
	std::vector<SerializedTask> allTask;
	for (auto i = document.begin(); i != document.end(); ++i) {
		allTask.push_back(SerializationOperation::deserialize(*i));
	}
	return allTask;
}

bool DataStore::saveData() {
	bool status = document.save_file(FILE_PATH.c_str());
	return status;
}

void DataStore::loadData() {
	bool isInitialized =
		document.first_child().type() != pugi::xml_node_type::node_null;
	if (!isInitialized) {
		pugi::xml_parse_result status = document.load_file(FILE_PATH.c_str());

		// Not sure if the if block below is necessary
		if (status == pugi::xml_parse_status::status_file_not_found) {
			document.reset();
		}
	}
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

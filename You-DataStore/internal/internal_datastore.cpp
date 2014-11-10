//@author A0114171W
#include "stdafx.h"
#include <boost/format.hpp>
#include "../../You-Utils/log.h"
#include "../exception.h"
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

using Log = Utils::Log;

const char* DataStore::FILE_PATH = "data.xml";
const std::wstring DataStore::ROOT_NODE_NAME = std::wstring(L"You");

const std::wstring DataStore::LOG_CATEGORY = L"[DS][InternalDataStore]";

DataStore& DataStore::get() {
	static DataStore store;
	return store;
}

You::DataStore::Transaction DataStore::begin() {
	Log::info << LOG_CATEGORY << L": Begin transaction.";
	You::DataStore::Transaction result;
	transactionStack.push(std::weak_ptr<Internal::Transaction>(result));

	return result;
}

void DataStore::onTransactionCommit(Transaction& transaction) {
	// Only transaction on top of the stack may be committed
	assert(*transactionStack.top().lock() == transaction);
	auto self = transactionStack.top();

	if (transactionStack.size() == 1) {
		// it is the only active transaction, execute the operations and save
		Log::info << LOG_CATEGORY << L": Committing transaction to data.xml.";

		// Create a copy of the current data, so that if transaction fails,
		// the original document is not corrupted
		pugi::xml_document temp;
		temp.reset(document);
		pugi::xml_node tempRoot = BranchOperation::get(temp, ROOT_NODE_NAME.c_str());
		executeTransaction(transaction, tempRoot);

		// Commit successful, overwrite the document and root
		document.reset(temp);
		root = BranchOperation::get(document, ROOT_NODE_NAME.c_str());
		bool isSaved = saveData();
		if (isSaved) {
			Log::info << LOG_CATEGORY << L": data.xml is saved successfully.";
		} else {
			Log::error << LOG_CATEGORY << L": An I/O error occured, data.xml "
				L"is not saved.";
			throw IOException();
		}
		transactionStack.pop();
	} else {
		// There is a transaction before it that is yet to be committed.
		// Merge with that transaction
		Log::info << LOG_CATEGORY << L": Merging transaction.";
		transactionStack.pop();
		auto below = transactionStack.top().lock();
		below->mergeOperationsQueue(transaction.operationsQueue);
		below->mergeOperationsQueue(transaction.mergedOperationsQueue);
	}
}

void DataStore::onTransactionRollback(Transaction& transaction) {
	// Can only rollback the latest transaction
	assert(*(transactionStack.top().lock()) == transaction);
	Log::info << LOG_CATEGORY << L": Popping transaction from the stack.";
	transactionStack.pop();
}

void DataStore::post(std::wstring branch, std::wstring id,
	const KeyValuePairs& kvp) {
	assert(!transactionStack.empty());
	Log::info << (boost::wformat(L"%1%: POST to %2%, id = %3%.") %
		LOG_CATEGORY % branch % id).str();

	std::unique_ptr<Internal::Operation> operation =
		std::make_unique<Internal::PostOperation>(branch, id, kvp);

	auto transaction = transactionStack.top().lock();
	assert(transaction);  // Checks if the pointer is valid
	transaction->push(std::move(operation));
}

void DataStore::put(std::wstring branch, std::wstring id,
	const KeyValuePairs& kvp) {
	assert(!transactionStack.empty());
	Log::info << (boost::wformat(L"%1%: PUT to %2%, id = %3%.") %
		LOG_CATEGORY % branch % id).str();

	std::unique_ptr<Internal::Operation> operation =
		std::make_unique<Internal::PutOperation>(branch, id, kvp);

	auto transaction = transactionStack.top().lock();
	assert(transaction);  // Checks if the pointer is valid
	transaction->push(std::move(operation));
}

void DataStore::erase(std::wstring branch, std::wstring id) {
	assert(!transactionStack.empty());
	Log::info << (boost::wformat(L"%1%: ERASE to %2%, id = %3%.") %
		LOG_CATEGORY % branch % id).str();

	std::unique_ptr<Internal::Operation> operation =
		std::make_unique<Internal::EraseOperation>(branch, id);

	auto transaction = transactionStack.top().lock();
	assert(transaction);  // Checks if the pointer is valid
	transaction->push(std::move(operation));
}

std::vector<KeyValuePairs> DataStore::getAll(std::wstring nodeName) {
	Log::info << LOG_CATEGORY << L": Getting all " << nodeName;
	loadData();
	pugi::xml_node dataNode = BranchOperation::get(root, nodeName);
	std::vector<KeyValuePairs> allData;
	for (auto i = dataNode.begin(); i != dataNode.end(); ++i) {
		allData.push_back(SerializationOperation::deserialize(*i));
	}
	return allData;
}

void DataStore::wipeData() {
	Log::warning << LOG_CATEGORY << L": All DataStore state, including data.xml"
		L"and the XML document will be wiped.";
	document.reset();
	std::remove(FILE_PATH);
}

bool DataStore::saveData() {
	bool status = document.save_file(FILE_PATH);
	return status;
}

void DataStore::loadData() {
	bool isInitialized = !document.first_child().empty();
	if (!isInitialized) {
		pugi::xml_parse_result loadStatus = document.load_file(FILE_PATH);
		bool loadSuccessful = loadStatus;
		bool isFirstLoad =
			loadStatus.status == pugi::xml_parse_status::status_file_not_found;
		if (!loadSuccessful && !isFirstLoad) {
			onXmlParseResult(loadStatus);
		} else {
			root = BranchOperation::get(document, ROOT_NODE_NAME.c_str());
		}
	}
}

void DataStore::executeTransaction(Transaction& transaction,
	pugi::xml_node& node) {
	Log::debug << LOG_CATEGORY << L": Executing operationsQueue.";
	for (auto operation = transaction.operationsQueue.begin();
		operation != transaction.operationsQueue.end();
		++operation) {
		bool status = operation->run(node);
		if (!status) {
			transaction.rollback();
			assert(false);
		}
	}

	Log::debug << LOG_CATEGORY << L": Executing mergedOperationsQueue.";
	for (auto mergedOperation = transaction.mergedOperationsQueue.begin();
		mergedOperation != transaction.mergedOperationsQueue.end();
		++mergedOperation) {
		bool status = mergedOperation->run(node);
		if (!status) {
			transaction.rollback();
			assert(false);
		}
	}
}

void DataStore::onXmlParseResult(const pugi::xml_parse_result& result) {
	bool isIoError =
		result.status == pugi::xml_parse_status::status_io_error ||
		result.status == pugi::xml_parse_status::status_out_of_memory ||
		result.status == pugi::xml_parse_status::status_internal_error;
	if (isIoError) {
		Log::error << LOG_CATEGORY << L": An I/O error occured, data.xml "
			L"is not loaded.";
		throw IOException();
	} else {
		Log::error << LOG_CATEGORY << L": " << result.description();
		throw NotWellFormedXmlException();
	}
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

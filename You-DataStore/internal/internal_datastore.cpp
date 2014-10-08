#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "operation.h"
#include "operations/post_operation.h"
#include "operations/put_operation.h"
#include "operations/erase_operation.h"
#include "internal_datastore.h"

namespace You {
namespace DataStore {
namespace Internal {

const std::wstring InternalDataStore::FILE_PATH = std::wstring(L"data.xml");

bool InternalDataStore::post(TaskId rawId, const SerializedTask& sTask) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::PostOperation>(rawId, sTask);
	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}

	return false;
}

bool InternalDataStore::put(TaskId rawId, const SerializedTask& sTask) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::PutOperation>(rawId, sTask);
	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}

	return false;
}

bool InternalDataStore::erase(TaskId rawId) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::EraseOperation>(rawId);
	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}

	return false;
}

SerializedTask InternalDataStore::getTask(TaskId rawId) {
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	pugi::xml_node toGet =
		document.find_child_by_attribute(L"id", taskId.c_str());
	
	return SerializationOperation::deserialize(toGet);
}

std::vector<SerializedTask> InternalDataStore::getAllTask() {
	loadData();
	std::vector<SerializedTask> allTask;
	for (auto i = document.begin(); i != document.end(); ++i) {
		pugi::xml_node test = *i;
		// FOR SOME REASON IT IS A PCDATA
		if (test.type() == pugi::xml_node_type::node_pcdata) {
			throw "Fffff";
		}
		allTask.push_back(SerializationOperation::deserialize(*i));
	}
	return allTask;
}

bool InternalDataStore::saveData() {
	bool status = document.save_file(FILE_PATH.c_str());
	return status;
}

void InternalDataStore::loadData() {
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

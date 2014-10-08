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
#if 0
	// Consider changing parameter to std::wstring altogether
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	if (document.find_child_by_attribute(L"id", taskId.c_str())) {
		return false;
	}
	pugi::xml_node newTask = document.append_child(L"task");

	pugi::xml_attribute id = newTask.append_attribute(L"id");
	id.set_value(taskId.c_str());
	serialize(sTask, newTask);
	return true;
#endif
	return false;
}

bool InternalDataStore::put(TaskId rawId, const SerializedTask& sTask) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::PutOperation>(rawId, sTask);
	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
#if 0
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	pugi::xml_node toEdit =
		document.find_child_by_attribute(L"id", taskId.c_str());
	if (!toEdit) {
		return false;
	}
	document.remove_child(toEdit);
	return post(rawId, sTask);
#endif
	return false;
}

bool InternalDataStore::erase(TaskId rawId) {
	std::unique_ptr<Internal::IOperation> operation =
		std::make_unique<Internal::EraseOperation>(rawId);
	if (auto transaction = transactionStack.top().lock()) {
		transaction->push(std::move(operation));
	}
#if 0
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	pugi::xml_node toErase =
		document.find_child_by_attribute(L"id", taskId.c_str());
	return document.remove_child(toErase);
#endif
	return false;
}

SerializedTask InternalDataStore::getTask(TaskId rawId) {
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	pugi::xml_node toGet =
		document.find_child_by_attribute(L"id", taskId.c_str());
	SerializedTask stask = deserialize(toGet);
	return stask;
}

std::vector<SerializedTask> InternalDataStore::getAllTask() {
#if 0
	loadData();
	std::vector<SerializedTask> allTask;
	for (auto i = document.begin(); i != document.end(); ++i) {
		pugi::xml_node test = *i;
		// FOR SOME REASON IT IS A PCDATA
		if (test.type() == pugi::xml_node_type::node_pcdata) {
			throw "Fffff";
		}
		allTask.push_back(deserialize(*i));
	}
	return allTask;
#endif
	return std::vector<SerializedTask>();
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

void InternalDataStore::serialize(const SerializedTask& stask,
	pugi::xml_node& node) {
	for (auto iter = stask.begin(); iter != stask.end(); ++iter) {
		pugi::xml_node keyNode =
			node.append_child(iter->first.c_str());
		pugi::xml_node keyValue =
			keyNode.append_child(pugi::xml_node_type::node_pcdata);
		keyValue.set_value(iter->second.c_str());
	}
}

SerializedTask InternalDataStore::deserialize(const pugi::xml_node& taskNode) {
	SerializedTask stask;
	for (auto iter = taskNode.begin(); iter != taskNode.end(); ++iter) {
		stask.insert(KeyValuePair(Key(iter->name()),
			Value(iter->child_value())));
	}
	return stask;
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

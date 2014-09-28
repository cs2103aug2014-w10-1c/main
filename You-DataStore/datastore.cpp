#include "stdafx.h"
#include "datastore.h"

namespace You {
namespace DataStore {

const std::wstring DataStore::FILE_PATH = std::wstring(L"data.xml");

bool DataStore::post(TaskId rawId, const STask& sTask) {
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	if (document.find_child_by_attribute(L"id", taskId.c_str())) {
		return false;
	}
	pugi::xml_node newTask = document.append_child(L"task");

	pugi::xml_attribute id = newTask.append_attribute(L"id");
	id.set_value(taskId.c_str());

	for each (KeyValuePair kvp in sTask) {
		pugi::xml_node keyNode =
			newTask.append_child(kvp.first.c_str());
		pugi::xml_node keyValue =
			keyNode.append_child(pugi::xml_node_type::node_pcdata);
		keyValue.set_value(kvp.second.c_str());
	}
	return true;
}

bool DataStore::put(TaskId rawId, const STask& sTask) {
	std::wstring taskId = boost::lexical_cast<std::wstring>(rawId);
	pugi::xml_node toEdit =
		document.find_child_by_attribute(L"id", taskId.c_str());
	if (!toEdit) {
		return false;
	}
	document.remove_child(toEdit);
	return post(rawId, sTask);
}

bool You::DataStore::DataStore::saveData() {
	bool status = document.save_file(FILE_PATH.c_str());
	return status;
}

void You::DataStore::DataStore::loadData() {
	pugi::xml_parse_result status = document.load_file(FILE_PATH.c_str());

	// Not sure if the if block below is necessary
	if (status == pugi::xml_parse_status::status_file_not_found) {
		document.reset();
	}
}

}  // namespace DataStore
}  // namespace You

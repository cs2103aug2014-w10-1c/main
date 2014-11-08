//@author A0114171W
#include "stdafx.h"
#include "internal/internal_datastore.h"
#include "internal/constants.h"
#include "datastore.h"

namespace You {
namespace DataStore {

const std::wstring DataStore::TASKS_NODE		= std::wstring(L"tasks");
const std::wstring DataStore::RESOURCES_NODE	= std::wstring(L"resources");

DataStore& DataStore::get() {
	static DataStore ds;
	return ds;
}

Transaction DataStore::begin() {
	return Internal::DataStore::get().begin();
}

void DataStore::post(TaskId taskId, const KeyValuePairs& task) {
	std::wstring stringId = boost::lexical_cast<std::wstring>(taskId);
	Internal::DataStore::get().post(TASKS_NODE, stringId, task);
}

void DataStore::put(TaskId taskId, const KeyValuePairs& task) {
	std::wstring stringId = boost::lexical_cast<std::wstring>(taskId);
	Internal::DataStore::get().put(TASKS_NODE, stringId, task);
}

void DataStore::erase(TaskId taskId) {
	std::wstring stringId = boost::lexical_cast<std::wstring>(taskId);
	Internal::DataStore::get().erase(TASKS_NODE, stringId);
}

void DataStore::post(std::wstring resourceId, const KeyValuePairs& kvp) {
	Internal::DataStore::get().post(RESOURCES_NODE, resourceId, kvp);
}

void DataStore::put(std::wstring resourceId, const KeyValuePairs& kvp) {
	Internal::DataStore::get().put(RESOURCES_NODE, resourceId, kvp);
}

void DataStore::erase(std::wstring resourceId) {
	Internal::DataStore::get().erase(RESOURCES_NODE, resourceId);
}

std::vector<KeyValuePairs> DataStore::getAllTasks() {
	return Internal::DataStore::get().getAll(TASKS_NODE);
}

std::vector<KeyValuePairs> DataStore::getAllResources() {
	return Internal::DataStore::get().getAll(RESOURCES_NODE);
}

void DataStore::wipeData() {
	Internal::DataStore::get().wipeData();
}

}  // namespace DataStore
}  // namespace You

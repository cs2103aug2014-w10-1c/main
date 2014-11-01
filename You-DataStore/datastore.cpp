//@author A0114171W
#include "stdafx.h"
#include "internal/internal_datastore.h"
#include "internal/constants.h"
#include "datastore.h"

namespace You {
namespace DataStore {

DataStore& DataStore::get() {
	static DataStore ds;
	return ds;
}

Transaction DataStore::begin() {
	return Internal::DataStore::get().begin();
}

void DataStore::post(TaskId taskId, const KeyValuePairs& task) {
	std::wstring stringId = boost::lexical_cast<std::wstring>(taskId);
	Internal::DataStore::get().post(Internal::TASKS_NODE, stringId, task);
}

void DataStore::put(TaskId taskId, const KeyValuePairs& task) {
	std::wstring stringId = boost::lexical_cast<std::wstring>(taskId);
	Internal::DataStore::get().put(Internal::TASKS_NODE, stringId, task);
}

void DataStore::erase(TaskId taskId) {
	std::wstring stringId = boost::lexical_cast<std::wstring>(taskId);
	Internal::DataStore::get().erase(Internal::TASKS_NODE, stringId);
}

void DataStore::post(std::wstring resourceId, const KeyValuePairs& kvp) {
	Internal::DataStore::get().post(Internal::RESOURCES_NODE, resourceId, kvp);
}

void DataStore::put(std::wstring resourceId, const KeyValuePairs& kvp) {
	Internal::DataStore::get().put(Internal::RESOURCES_NODE, resourceId, kvp);
}

void DataStore::erase(std::wstring resourceId) {
	Internal::DataStore::get().erase(Internal::RESOURCES_NODE, resourceId);
}

std::vector<KeyValuePairs> DataStore::getAllTasks() {
	return Internal::DataStore::get().getAll(Internal::TASKS_NODE);
}

std::vector<KeyValuePairs> DataStore::getAllResources() {
	return Internal::DataStore::get().getAll(Internal::RESOURCES_NODE);
}

void DataStore::wipeData() {
	Internal::DataStore::get().wipeData();
}

}  // namespace DataStore
}  // namespace You

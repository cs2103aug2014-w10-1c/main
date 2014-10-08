#include "stdafx.h"
#include "internal/internal_datastore.h"
#include "datastore.h"

namespace You {
namespace DataStore {

DataStore& DataStore::get() {
	static DataStore ds;
	return ds;
}

Transaction DataStore::begin() {
	return Internal::InternalDataStore::get().begin();
}

void DataStore::post(TaskId taskId, const SerializedTask& task) {
	Internal::InternalDataStore::get().post(taskId, task);
}

void DataStore::put(TaskId taskId, const SerializedTask& task) {
	Internal::InternalDataStore::get().put(taskId, task);
}

void DataStore::erase(TaskId taskId) {
	Internal::InternalDataStore::get().erase(taskId);
}

std::vector<SerializedTask> DataStore::getAllTasks() {
	return Internal::InternalDataStore::get().getAllTask();
}

}  // namespace DataStore
}  // namespace You

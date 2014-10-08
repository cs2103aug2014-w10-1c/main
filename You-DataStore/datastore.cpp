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
	return Internal::DataStore::get().begin();
}

void DataStore::post(TaskId taskId, const SerializedTask& task) {
	Internal::DataStore::get().post(taskId, task);
}

void DataStore::put(TaskId taskId, const SerializedTask& task) {
	Internal::DataStore::get().put(taskId, task);
}

void DataStore::erase(TaskId taskId) {
	Internal::DataStore::get().erase(taskId);
}

std::vector<SerializedTask> DataStore::getAllTasks() {
	return Internal::DataStore::get().getAllTask();
}

}  // namespace DataStore
}  // namespace You

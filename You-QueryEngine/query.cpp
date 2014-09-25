#include "stdafx.h"

#include "query.h"
#include "schema.h"

namespace You {
namespace QueryEngine {

std::unique_ptr<Query::AddTask> AddTask(
	Task::Description description, Task::Time deadline,
	Task::Priority priority, Task::Dependencies dependencies) {
	std::unique_ptr<Query::AddTask> addTask(new Query::AddTask());
	addTask->description = description;
	addTask->deadline = deadline;
	addTask->priority = priority;
	addTask->dependencies = dependencies;
	return addTask;
}

Response executeQuery(std::unique_ptr<Query::AddTask> query) {
	return std::wstring(L"ADD");
}

std::unique_ptr<Query::GetTask> GetTask(Task::ID id) {
	std::unique_ptr<Query::GetTask> getTask(new Query::GetTask());
	getTask->taskID = id;
	return getTask;
}

Response executeQuery(std::unique_ptr<Query::GetTask> query) {
	return std::wstring(L"GET");
}

std::unique_ptr<Query::DeleteTask> DeleteTask(Task::ID taskID) {
	std::unique_ptr<Query::DeleteTask> deleteTask(new Query::DeleteTask());
	deleteTask->taskID = taskID;
	return deleteTask;
}

Response executeQuery(std::unique_ptr<Query::DeleteTask> query) {
	return std::wstring(L"DELETE");
}

}  // namespace QueryEngine
}  // namespace You


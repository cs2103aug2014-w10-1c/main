#include "stdafx.h"

#include "query.h"
#include "schema.h"

namespace You {
namespace QueryEngine {

std::unique_ptr<Query::AddTask> AddTask(Task::Description description
	, Task::Time deadline
	, Task::Priority priority
	, Task::Dependencies dependencies
	) {
	using Query::AddTask;
	AddTask* addTask = new AddTask();
	addTask->description = description;
	addTask->deadline = deadline;
	addTask->priority = priority;
	addTask->dependencies = dependencies;
	return std::unique_ptr<Query::AddTask>(addTask);
}

Response executeQuery(std::unique_ptr<Query::AddTask> query) {
	using Query::AddTask;
	return std::wstring(L"ADD");
}

std::unique_ptr<Query::GetTask> GetTask(Task::ID id) {
	using Query::GetTask;
	GetTask* getTask = new GetTask();
	getTask->taskID = id;
	return std::unique_ptr<GetTask>(getTask);
}

Response executeQuery(std::unique_ptr<Query::GetTask> query) {
	using Query::GetTask;
	return std::wstring(L"GET");
}

std::unique_ptr<Query::DeleteTask> DeleteTask(Task::ID taskID) {
	using Query::DeleteTask;
	DeleteTask* deleteTask = new DeleteTask();
	deleteTask->taskID = taskID;
	return std::unique_ptr<DeleteTask>(deleteTask);
}

Response executeQuery(std::unique_ptr<Query::DeleteTask> query) {
	using Query::DeleteTask;
	return std::wstring(L"DELETE");
}

}  // namespace QueryEngine
}  // namespace You


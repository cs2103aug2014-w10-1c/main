#include "stdafx.h"

#include "query.h"
#include "internal/schema.h"

namespace You {
namespace QueryEngine {

std::unique_ptr<Query> AddTask(Task::Description description
	, Task::Time deadline
	, Task::Priority priority
	, Task::Dependencies dependencies
	) {
	using Internal::Query::AddTask;
	return std::make_unique<AddTask>(description,
		deadline, priority, dependencies);
}

std::unique_ptr<Query> GetTask(Task::ID id) {
	using Internal::Query::GetTask;
	return std::make_unique<GetTask>(id);
}

std::unique_ptr<Query> DeleteTask(Task::ID taskID) {
	using Internal::Query::DeleteTask;
	return std::make_unique<DeleteTask>(taskID);
}

Response executeQuery(std::unique_ptr<Query> query) {
	using Internal::Query::AddTask;
	using Internal::Query::GetTask;
	using Internal::Query::DeleteTask;
}

}  // namespace QueryEngine
}  // namespace You


#include "stdafx.h"

#include "query.h"
#include "internal/schema.h"

namespace You {
namespace QueryEngine {

std::shared_ptr<Query> AddTask(Task::Description description
	, Task::Time deadline
	, Task::Priority priority
	, Task::Dependencies dependencies
	) {
	using Internal::Query::AddTask;
	return std::make_shared<AddTask>(description, deadline,
		priority, dependencies);
}

std::shared_ptr<Query> GetTask(Task::ID id) {
	using Internal::Query::GetTask;
	return std::make_shared<GetTask>(id);
}

std::shared_ptr<Query> DeleteTask(Task::ID taskID) {
	using Internal::Query::DeleteTask;
	return std::make_shared<DeleteTask>(taskID);
}

}  // namespace QueryEngine
}  // namespace You


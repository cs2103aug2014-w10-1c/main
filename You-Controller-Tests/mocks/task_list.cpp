#include "stdafx.h"
#include "task_list.h"

namespace You {
namespace Controller {
namespace UnitTests {
namespace Mocks {

TaskList::TaskList() {
	std::unique_ptr<QueryEngine::Query> q = QueryEngine::AddTask(
		L"meh",
		QueryEngine::Task::DEFAULT_DEADLINE,
		QueryEngine::Task::DEFAULT_PRIORITY,
		QueryEngine::Task::Dependencies());

	QueryEngine::Response r = QueryEngine::executeQuery(std::move(q));
	Task task = boost::get<Task>(r);
	emplace_back(task);
}

}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

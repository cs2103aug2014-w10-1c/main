#include "stdafx.h"
#include "task_list.h"

namespace You {
namespace Controller {
namespace UnitTests {
namespace Mocks {

TaskList::TaskList(size_t count) {
	std::generate_n(std::back_inserter<TaskList>(*this),
		count, std::bind(&TaskList::createTask, this));
}

Task TaskList::createTask() {
	std::unique_ptr<QueryEngine::Query> q = QueryEngine::AddTask(
		L"meh",
		boost::posix_time::second_clock::local_time() +
			boost::posix_time::hours(size()),
		QueryEngine::Task::DEFAULT_PRIORITY,
		QueryEngine::Task::Dependencies());

	QueryEngine::Response r = QueryEngine::executeQuery(std::move(q));
	return boost::get<Task>(r);
}

}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

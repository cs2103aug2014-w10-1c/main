#include "stdafx.h"
#include <random>
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
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 1);

	std::unique_ptr<QueryEngine::Query> q = QueryEngine::AddTask(
		L"meh " + boost::lexical_cast<std::wstring>(size()),
		boost::posix_time::second_clock::local_time() +
			boost::posix_time::hours(size()),
			static_cast<QueryEngine::Task::Priority>(distribution(generator)),
		QueryEngine::Task::Dependencies());

	QueryEngine::Response r = QueryEngine::executeQuery(std::move(q));
	return boost::get<Task>(r);
}

}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

/// \author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"

#include "internal/task_builder.h"
#include "internal/exception.h"
#include "common.h"
#include "exclusions.h"
#include "api.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using boost::gregorian::date;
using boost::gregorian::max_date_time;

using You::QueryEngine::Task;
using You::QueryEngine::Response;
using You::QueryEngine::AddTask;
using You::QueryEngine::executeQuery;

using Task = You::QueryEngine::Task;
using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;

TEST_CLASS(QueryEngineTests) {
	TEST_METHOD(constructAddTaskQuery) {
		Task::Description desc = L"Learn Haskell Lens";
		Task::Time dead = Task::NEVER;
		Task::Priority prio = Task::Priority::IMPORTANT;
		Task::Dependencies dep = Task::Dependencies();
		auto query = AddTask(desc, dead, prio, dep);
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(constructFilterTaskQuery) {
		std::vector<Task::ID> emptyVec;
		auto query = FilterTask(Filter::idIsIn(emptyVec));
		Assert::IsNotNull(&query);
		query = FilterTask(Filter::anyTask());
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(constructDeleteTaskQuery) {
		auto query = DeleteTask(Task::DEFAULT_ID);
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(constructEditTaskQuery) {
		auto query = UpdateTask(Task::DEFAULT_ID,
				Task::DEFAULT_DESCRIPTION, Task::DEFAULT_DEADLINE,
				Task::DEFAULT_PRIORITY, Task::DEFAULT_DEPENDENCIES);
		Assert::IsNotNull(&query);
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

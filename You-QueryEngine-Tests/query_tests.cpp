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
		Task task = TaskBuilder::get()
			.description(desc).deadline(dead)
			.priority(prio).dependencies(dep);
		Response resp = executeQuery(AddTask(desc, dead, prio, dep));
		Assert::IsTrue(boost::get<Task>(resp) == (task));
	}

	TEST_METHOD(constructFilterTaskQuery) {
		Response resp = executeQuery(FilterTask(Filter::anyTask()));
		Assert::IsTrue(boost::get<std::vector<Task>>(resp).empty());
		std::vector<Task::ID> emptyVec;
		resp = executeQuery(FilterTask(Filter::idIsIn(emptyVec)));
		Assert::IsTrue(boost::get<std::vector<Task>>(resp).empty());
	}

	TEST_METHOD(constructDeleteTaskQuery) {
		Response resp = executeQuery(DeleteTask(Task::DEFAULT_ID));
		Assert::IsNotNull(&resp);
	}

	TEST_METHOD(constructEditTaskQuery) {
		using You::QueryEngine::Internal::EmptyTaskDescriptionException;
		Assert::ExpectException<EmptyTaskDescriptionException>([] {
			executeQuery(UpdateTask(Task::DEFAULT_ID,
				Task::DEFAULT_DESCRIPTION, Task::DEFAULT_DEADLINE,
				Task::DEFAULT_PRIORITY, Task::DEFAULT_DEPENDENCIES));
		});
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

/// \author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"

#include "internal/task_builder.h"
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
		Assert::IsTrue(boost::get<Task>(resp).isStrictEqual(task));
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

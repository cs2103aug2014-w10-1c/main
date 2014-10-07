/// \author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"
#include "common.h"
#include "exclusions.h"

#include "internal/task_builder.h"
#include "internal/exception.h"
#include "internal/state.h"
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
	const Task::Description desc = L"Learn Haskell Lens";
	const Task::Description desc2 = L"Learn me";
	const Task::Time dead = Task::NEVER;
	const Task::Priority prio = Task::Priority::IMPORTANT;
	const Task::Dependencies dep = Task::Dependencies();

	TEST_METHOD(constructAddTaskQuery) {
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

	TEST_METHOD(executeAddQuery) {
		Internal::State::clear();
		for (int i = 1; i <= 5; i++) {
			auto query = AddTask(desc, dead, prio, dep);
			auto response = executeQuery(std::move(query));
			std::size_t newSize = Internal::State::get().graph().getTaskList().size();
			Assert::AreEqual(newSize, std::size_t(i));
			Assert::AreEqual(boost::get<Task>(response).getDescription(), desc);
		}
		Internal::State::clear();
	}

	TEST_METHOD(executeEditQuery) {
		Internal::State::clear();
		auto query = AddTask(desc, dead, prio, dep);
		auto response = executeQuery(std::move(query));
		query = UpdateTask(
			boost::get<Task>(response).getID(), desc2,
			Task::DEFAULT_DEADLINE, Task::DEFAULT_PRIORITY,
			Task::DEFAULT_DEPENDENCIES);
		response = executeQuery(std::move(query));
		Task::ID id = boost::get<Task>(response).getID();
		Assert::AreEqual(boost::get<Task>(response)
			.getDescription(), desc2);
		Assert::AreEqual(Internal::State::get().graph().getTask(id)
			.getDescription(), desc2);
		Internal::State::clear();
	}

	TEST_METHOD(executeMarkTaskQuery) {
		Internal::State::clear();
		auto query = AddTask(desc, dead, prio, dep);
		auto response = executeQuery(std::move(query));
		auto task = boost::get<Task>(response);
		Assert::IsFalse(task.isCompleted());

		query = UpdateTask(task.getID(), true);
		response = executeQuery(std::move(query));
		task = boost::get<Task>(response);
		Assert::IsTrue(task.isCompleted());
		Assert::IsTrue(Internal::State::get().graph()
			.getTask(task.getID()).isCompleted());

		query = UpdateTask(task.getID(), false);
		response = executeQuery(std::move(query));
		task = boost::get<Task>(response);
		Assert::IsFalse(task.isCompleted());
		Assert::IsFalse(Internal::State::get().graph()
			.getTask(task.getID()).isCompleted());
		Internal::State::clear();
	}

	TEST_METHOD(executeDeleteQuery) {
		Internal::State::clear();
		auto query = AddTask(desc, dead, prio, dep);
		auto response = executeQuery(std::move(query));
		query = DeleteTask(boost::get<Task>(response).getID());
		response = executeQuery(std::move(query));
		Assert::AreEqual(Internal::State::get().graph()
			.getTaskList().size(), std::size_t(0));
		Internal::State::clear();
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

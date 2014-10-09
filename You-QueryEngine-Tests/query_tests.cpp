/// \author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"
#include "common.h"
#include "exclusions.h"

#include <type_traits>
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
using You::QueryEngine::QueryEngine;

using Task = You::QueryEngine::Task;
using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;

TEST_CLASS(QueryEngineTests) {
	const Task::Description desc = L"Learn Haskell Lens";
	const Task::Description desc2 = L"Learn me";
	const Task::Time dead = Task::NEVER;
	const Task::Priority prio = Task::Priority::HIGH;
	const Task::Dependencies dep = Task::Dependencies();

	TEST_METHOD(queryEngineIsUtilityClass) {
		static_assert(
			!std::is_trivially_default_constructible<QueryEngine>::value,
		"QueryEngine cannot be default constructed");
	}

	TEST_METHOD(constructAddTaskQuery) {
		auto query = QueryEngine::AddTask(desc, dead, prio, dep);
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(constructFilterTaskQuery) {
		std::vector<Task::ID> emptyVec;
		auto query = QueryEngine::FilterTask(Filter::idIsIn(emptyVec));
		Assert::IsNotNull(&query);
		query = QueryEngine::FilterTask(Filter::anyTask());
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(constructDeleteTaskQuery) {
		auto query = QueryEngine::DeleteTask(Task::DEFAULT_ID);
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(constructEditTaskQuery) {
		auto query = QueryEngine::UpdateTask(Task::DEFAULT_ID,
				Task::DEFAULT_DESCRIPTION, Task::DEFAULT_DEADLINE,
				Task::DEFAULT_PRIORITY, Task::DEFAULT_DEPENDENCIES);
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(executeAddQuery) {
		Internal::State::clear();
		for (int i = 1; i <= 5; i++) {
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			std::size_t newSize = Internal::State::get().graph().getTaskList().size();
			Assert::AreEqual(newSize, std::size_t(i));
			Assert::AreEqual(boost::get<Task>(response).getDescription(), desc);
		}
		Internal::State::clear();
	}

	TEST_METHOD(executeEditQuery) {
		Internal::State::clear();

		#pragma region Add one task
		Task task;
		1; {
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		#pragma region Update the description
		2; {
			auto query = QueryEngine::UpdateTask(
				task.getID(), desc2,
				Task::DEFAULT_DEADLINE, Task::DEFAULT_PRIORITY,
				Task::DEFAULT_DEPENDENCIES);
			auto response = QueryEngine::executeQuery(std::move(query));

			Task::ID id = boost::get<Task>(response).getID();
			Assert::AreEqual(boost::get<Task>(response)
				.getDescription(), desc2);
			Assert::AreEqual(Internal::State::get().graph().getTask(id)
				.getDescription(), desc2);
		}
		#pragma endregion

		Internal::State::clear();
	}

	TEST_METHOD(executeMarkTaskQuery) {
		Internal::State::clear();

		#pragma region Add one Task
		Task task;
		1; {
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsFalse(task.isCompleted());
		}
		#pragma endregion

		#pragma region Mark the task added as done
		2; {
			auto query = QueryEngine::UpdateTask(task.getID(), true);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsTrue(task.isCompleted());
			Assert::IsTrue(Internal::State::get().graph()
				.getTask(task.getID()).isCompleted());
		}
		#pragma endregion

		#pragma region Mark the task added as undone again
		3; {
			auto query = QueryEngine::UpdateTask(task.getID(), false);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsFalse(task.isCompleted());
			Assert::IsFalse(Internal::State::get().graph()
				.getTask(task.getID()).isCompleted());
		}
		#pragma endregion

		Internal::State::clear();
	}

	TEST_METHOD(executeDeleteQuery) {
		Internal::State::clear();
		#pragma region Add one task
		Task task;
		1; {
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}

		#pragma endregion

		#pragma region Delete the task
		2; {
			auto query = QueryEngine::DeleteTask(task.getID());
			auto response = QueryEngine::executeQuery(std::move(query));
			Assert::AreEqual(Internal::State::get().graph()
				.getTaskList().size(), std::size_t(0));
		}
		#pragma endregion
		Internal::State::clear();
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

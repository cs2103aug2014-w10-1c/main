//@author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"
#include "common.h"

#include <type_traits>
#include "../You-DataStore/datastore.h"
#include "mocks/task_list.h"
#include "exception.h"
#include "internal/controller/task_builder.h"
#include "internal/state.h"
#include "api.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

/// \cond Imports
namespace {
	using boost::gregorian::date;
	using boost::gregorian::max_date_time;

	using You::QueryEngine::Filter;
	using You::QueryEngine::Task;
	using You::QueryEngine::Response;
	using You::QueryEngine::QueryEngine;
}
/// \endcond

/// Test the main \ref QueryEngine API
TEST_CLASS(QueryEngineTests) {
	const Task::Description desc = L"Learn Haskell Lens";
	const Task::Description desc2 = L"Learn me";
	const Task::Time dead = Task::NEVER;
	const Task::Priority prio = Task::Priority::HIGH;

	TEST_METHOD_INITIALIZE(cleanupBeforeTest) {
		You::DataStore::DataStore::get().wipeData();
		Internal::State::clear();
	}

	TEST_METHOD_CLEANUP(cleanupAfterTest) {
		You::DataStore::DataStore::get().wipeData();
		Internal::State::clear();
	}

	TEST_METHOD(queryEngineIsUtilityClass) {
		static_assert(
			!std::is_trivially_default_constructible<QueryEngine>::value,
		"QueryEngine cannot be default constructed");
	}

	TEST_METHOD(constructAddTaskQuery) {
		auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(constructGetTaskQuery) {
		std::vector<Task::ID> emptyVec;
		auto query = QueryEngine::GetTask(Filter::idIsIn(emptyVec));
		Assert::IsNotNull(&query);
		query = QueryEngine::GetTask(Filter::anyTask());
		Assert::IsNotNull(&query);
	}

	TEST_METHOD(executeFilterQuery) {
		populateStateWithTasks(ID_ONE_TO_FIVE());
		std::vector<Task::ID> mustBeHere = { 0, 1, 2, 3, 10 };

		auto result = QueryEngine::executeQuery(
			QueryEngine::GetTask(Filter::idIsIn(mustBeHere)));
		std::size_t N_FILTERED = 3;

		Assert::AreEqual(boost::get<std::vector<Task>>(result).size(), N_FILTERED);
	}

	TEST_METHOD(executeAddQuery) {
		for (int i = 1; i <= 5; i++) {
			auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
			auto response = QueryEngine::executeQuery(std::move(query));
			std::size_t newSize = Internal::State::get().graph().asTaskList().size();
			Assert::AreEqual(newSize, std::size_t(i));
			Assert::AreEqual(boost::get<Task>(response).getDescription(), desc);
		}
	}

	TEST_METHOD(executeEditQuery) {
		#pragma region Add one task
		Task task;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		#pragma region Update the description
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::UpdateTask(
				task.getID(),
				desc2,
				boost::none,
				boost::none,
				boost::none,
				boost::none,
				boost::none,
				boost::none,
				boost::none);
			auto response = QueryEngine::executeQuery(std::move(query));

			Task::ID id = boost::get<Task>(response).getID();
			Assert::AreEqual(boost::get<Task>(response)
				.getDescription(), desc2);
			Assert::AreEqual(Internal::State::get().graph().getTask(id)
				.getDescription(), desc2);
		}
		#pragma endregion
	}

	TEST_METHOD(executeMarkTaskQuery) {
		#pragma region Add one Task
		Task task;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsFalse(task.isCompleted());
		}
		#pragma endregion

		#pragma region Mark the task added as done
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::UpdateTask(
				task.getID(),
				boost::none,
				boost::none,
				boost::none,
				boost::none,
				true,
				boost::none,
				boost::none,
				boost::none);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsTrue(task.isCompleted());
			Assert::IsTrue(Internal::State::get().graph()
				.getTask(task.getID()).isCompleted());
		}
		#pragma endregion

		#pragma region Mark the task added as undone again
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::UpdateTask(
				task.getID(),
				boost::none,
				boost::none,
				boost::none,
				boost::none,
				false,
				boost::none,
				boost::none,
				boost::none);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsFalse(task.isCompleted());
			Assert::IsFalse(Internal::State::get().graph()
				.getTask(task.getID()).isCompleted());
		}
		#pragma endregion
	}

	TEST_METHOD(executeDeleteQuery) {
		#pragma region Add one task
		Task task;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}

		#pragma endregion

		#pragma region Delete the task
		{   // NOLINT(whitespace/braces)
			auto query = QueryEngine::DeleteTask(task.getID());
			auto response = QueryEngine::executeQuery(std::move(query));
			Assert::AreEqual(Internal::State::get().graph()
				.asTaskList().size(), std::size_t(0));
		}
		#pragma endregion
	}

	TEST_METHOD(undoAddQuery) {
		#pragma region Add one task
		Task task;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));

		#pragma region Undo Last action
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(0));
		Assert::AreEqual(Internal::State::get().graph().getTaskCount(),
			0);
	}

	TEST_METHOD(undoDeleteQuery) {
		#pragma region Add one task
		Task task;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));

		#pragma region Delete one task
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::DeleteTask(task.getID());
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(2));

		#pragma region Undo last action
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().graph().getTaskCount(), 1);
	}

	TEST_METHOD(undoUpdateQuery) {
		#pragma region Add one task
		Task task;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(desc, dead, prio, {}, {});
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));

		#pragma region Update one task
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::UpdateTask(
				task.getID(),
				std::wstring(L"De geso"),
				task.getDeadline(),
				task.getPriority(),
				task.getDependencies(),
				boost::none,
				boost::none,
				boost::none,
				boost::none);
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(2));

		#pragma region Undo last action
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));
		Assert::AreEqual(Internal::State::get().graph().getTask(task.getID())
			.getDescription(), task.getDescription());
	}

	TEST_METHOD(undoWithEmptyStackShouldThrowException) {
		Assert::ExpectException<Exception::NotUndoAbleException>([] {
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		});
	}

	TEST_METHOD(setAttachment) {
		auto task = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(desc, dead, prio, {}, {})));
		task.setAttachment(L"index.html");
		task = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::UpdateTask(task)));
		Assert::AreEqual(task.getAttachment(), std::wstring(L"index.html"));
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

#include "stdafx.h"
#include "CppUnitTest.h"
#include "common.h"
#include "exclusions.h"

#include <type_traits>
#include "mocks/task_list.h"
#include "../You-Utils/exceptions/query_engine_exception.h"
#include "internal/controller/task_builder.h"
#include "internal/model.h"
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
			std::size_t newSize = Internal::State::get().graph().asTaskList().size();
			Assert::AreEqual(newSize, std::size_t(i));
			Assert::AreEqual(boost::get<Task>(response).getDescription(), desc);
		}
		Internal::State::clear();
	}

	TEST_METHOD(addTaskWithInvalidDependency) {
		Internal::State::clear();
		auto query = QueryEngine::AddTask(desc, dead, prio, { 1, 2, 3 });
		Assert::ExpectException<Exception::TaskNotFoundException>([&query] {
			QueryEngine::executeQuery(std::move(query));
		});
		Assert::AreEqual(0, Internal::State::get().graph().getTaskCount());
		Internal::State::clear();
	}

	TEST_METHOD(addTaskWithValidDependency) {
		Internal::State::clear();
		Task::ID insertedID;
		#pragma region Add a task
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			insertedID = boost::get<Task>(response).getID();
		}
		#pragma endregion

		#pragma region Add a task that depends on that task
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, { insertedID });
			QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(2, Internal::State::get().graph().getTaskCount());
		Internal::State::clear();
	}

	TEST_METHOD(executeEditQuery) {
		Internal::State::clear();

		#pragma region Add one task
		Task task;
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		#pragma region Update the description
		{  // NOLINT
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
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsFalse(task.isCompleted());
		}
		#pragma endregion

		#pragma region Mark the task added as done
		{  // NOLINT
			auto query = QueryEngine::UpdateTask(task.getID(), true);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
			Assert::IsTrue(task.isCompleted());
			Assert::IsTrue(Internal::State::get().graph()
				.getTask(task.getID()).isCompleted());
		}
		#pragma endregion

		#pragma region Mark the task added as undone again
		{  // NOLINT
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
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}

		#pragma endregion

		#pragma region Delete the task
		{   // NOLINT
			auto query = QueryEngine::DeleteTask(task.getID());
			auto response = QueryEngine::executeQuery(std::move(query));
			Assert::AreEqual(Internal::State::get().graph()
				.asTaskList().size(), std::size_t(0));
		}
		#pragma endregion
		Internal::State::clear();
	}

	TEST_METHOD(undoAddQuery) {
		Internal::State::clear();
		#pragma region Add one task
		Task task;
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));

		#pragma region Undo Last action
		{  // NOLINT
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(0));
		Assert::AreEqual(Internal::State::get().graph().getTaskCount(),
			0);

		Internal::State::clear();
	}

	TEST_METHOD(undoDeleteQuery) {
		Internal::State::clear();
		#pragma region Add one task
		Task task;
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));

		#pragma region Delete one task
		{  // NOLINT
			auto query = QueryEngine::DeleteTask(task.getID());
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(2));

		#pragma region Undo last action
		{  // NOLINT
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().graph().getTaskCount(), 1);
		Internal::State::clear();
	}

	TEST_METHOD(undoUpdateQuery) {
		Internal::State::clear();
		#pragma region Add one task
		Task task;
		{  // NOLINT
			auto query = QueryEngine::AddTask(desc, dead, prio, dep);
			auto response = QueryEngine::executeQuery(std::move(query));
			task = boost::get<Task>(response);
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));

		#pragma region Update one task
		{  // NOLINT
			auto query = QueryEngine::UpdateTask(task.getID(),
				L"De geso", task.getDeadline(), task.getPriority(),
				task.getDependencies());
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(2));

		#pragma region Undo last action
		{  // NOLINT
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		Assert::AreEqual(Internal::State::get().undoStack().size(),
			std::size_t(1));
		Assert::AreEqual(Internal::State::get().graph().getTask(task.getID())
			.getDescription(), task.getDescription());

		Internal::State::clear();
	}

	TEST_METHOD(undoWithEmptyStackShouldThrowException) {
		Internal::State::clear();
		Assert::ExpectException<Exception::NotUndoAbleException>([] {
			auto query = QueryEngine::Undo();
			auto response = QueryEngine::executeQuery(std::move(query));
		});
		Internal::State::clear();
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

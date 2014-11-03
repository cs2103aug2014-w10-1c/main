#include "stdafx.h"
#include "CppUnitTest.h"
#include "common.h"
#include "exclusions.h"

#include <type_traits>
#include "../You-DataStore/datastore.h"
#include "mocks/task_list.h"
#include "exception.h"
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

/// Test the advanced functionalities of QueryEngine API
TEST_CLASS(AdvancedQueryEngineTests) {
	const Task::Description desc = L"Learn Haskell Lens";
	const Task::Description desc2 = L"Learn me";
	const Task::Time dead = Task::NEVER;
	const Task::Priority prio = Task::Priority::HIGH;
	const Task::Dependencies dep = Task::Dependencies();
	const Task::Subtasks sub = Task::Subtasks();

	TEST_METHOD_INITIALIZE(cleanupBeforeTest) {
		You::DataStore::DataStore::get().wipeData();
		Internal::State::clear();
	}

	TEST_METHOD_CLEANUP(cleanupAfterTest) {
		You::DataStore::DataStore::get().wipeData();
		Internal::State::clear();
	}

	TEST_METHOD(executeBatchAddSubtaskQuery) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, dep, {})));
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, dep, {})));
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, dep, {})));
		auto query = QueryEngine::BatchAddSubTasks(
			desc, dead, prio, dep, std::move(childQueries));

		QueryEngine::executeQuery(std::move(query));

		auto inserted = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));

		// TODO(evansb) define ToString
		Assert::AreEqual(inserted.size(), static_cast<std::size_t>(1));
	}

	TEST_METHOD(executeBatchDeleteSubtaskQuery) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, dep, {})));

		Task parent;
		{  // NOLINT
			auto query = QueryEngine::BatchAddSubTasks(
				desc, dead, prio, dep, std::move(childQueries));
			QueryEngine::executeQuery(std::move(query));
		}

		Assert::AreEqual(Internal::State::get().graph().getTaskCount(),
			2);

		{  // NOLINT
			auto query = QueryEngine::BatchDeleteSubTasks(2);
			QueryEngine::executeQuery(std::move(query));
		}

		Assert::AreEqual(Internal::State::get().graph().getTaskCount(),
			0);
	}

	TEST_METHOD(executeBatchAddDependenciesQuery) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, {}, {})));
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, {}, {})));
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, {}, {})));
		auto query = QueryEngine::BatchAddDependencies(
			desc, dead, prio, std::move(childQueries), {});
		auto inserted = boost::get<Task>(
			QueryEngine::executeQuery(std::move(query)));

		Assert::IsTrue(inserted.isDependOn(3));
	}

	TEST_METHOD(markTaskAsDoneWillMarkItsChildrenAsDoneAlso) {
		#pragma region Add One Task
		Task::ID firstID;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(
				L"Hello World",
				Task::DEFAULT_DEADLINE,
				Task::DEFAULT_PRIORITY,
				Task::DEFAULT_DEPENDENCIES,
				Task::DEFAULT_SUBTASKS);
			auto response = QueryEngine::executeQuery(std::move(query));
			firstID = boost::get<Task>(response).getID();
		}
		#pragma endregion

		#pragma region Add another Task
		Task::ID secondID;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(
				L"Hello Marie",
				Task::DEFAULT_DEADLINE,
				Task::DEFAULT_PRIORITY,
				Task::DEFAULT_DEPENDENCIES,
				Task::DEFAULT_SUBTASKS);
			auto response = QueryEngine::executeQuery(std::move(query));
			secondID = boost::get<Task>(response).getID();
		}
		#pragma endregion

		#pragma region Make the first one depends on second one.
		{  // NOLINT(whitespace/braces)
			Task::Dependencies deps = { secondID };
			auto query = QueryEngine::UpdateTask(
				firstID,
				boost::none,
				boost::none,
				boost::none,
				deps,
				boost::none,
				boost::none,
				boost::none);
			QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		#pragma region Add another Task
		Task::ID thirdID;
		{  // NOLINT(whitespace/braces)
			auto query = QueryEngine::AddTask(
				L"Hello Wilson",
				Task::DEFAULT_DEADLINE,
				Task::DEFAULT_PRIORITY,
				Task::DEFAULT_DEPENDENCIES,
				Task::DEFAULT_SUBTASKS);
			auto response = QueryEngine::executeQuery(std::move(query));
			thirdID = boost::get<Task>(response).getID();
		}
		#pragma endregion

		#pragma region Make the second one depends on third one.
		{  // NOLINT(whitespace/braces)
			Task::Dependencies deps = { thirdID };
			auto query = QueryEngine::UpdateTask(
				secondID,
				boost::none,
				boost::none,
				boost::none,
				deps,
				boost::none,
				boost::none,
				boost::none);
			QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		#pragma region Mark the first one as completed.
		{  // NOLINT(whitespace/braces)
			Task::Dependencies deps = { secondID };
			auto query = QueryEngine::UpdateTask(
				firstID,
				boost::none,
				boost::none,
				boost::none,
				boost::none,
				true,
				boost::none,
				boost::none);
			QueryEngine::executeQuery(std::move(query));
		}
		#pragma endregion

		#pragma region Second and Third should be completed too
		auto secondOne = Internal::State::get().graph()
			.getTask(secondID);

		auto thirdOne = Internal::State::get().graph()
			.getTask(thirdID);

		Assert::IsTrue(secondOne.isCompleted());
		Assert::IsTrue(thirdOne.isCompleted());

		secondOne = Internal::State::get().sgraph()
			.getTask(secondID);

		thirdOne = Internal::State::get().sgraph()
			.getTask(thirdID);

		Assert::IsTrue(secondOne.isCompleted());
		Assert::IsTrue(thirdOne.isCompleted());
		#pragma endregion
	}

	TEST_METHOD(dontShowChildTaskIfTheParentIsAlreadyShown) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, Task::Priority::HIGH, dep, {})));
		QueryEngine::executeQuery(
			QueryEngine::BatchAddSubTasks(
				desc, dead, Task::Priority::HIGH, dep, std::move(childQueries)));
		auto getTask = QueryEngine::GetTask(Filter::highPriority());
		auto response = QueryEngine::executeQuery(std::move(getTask));
		auto result = boost::get<std::vector<Task>>(response);
		Assert::AreEqual(result.size(), static_cast<std::size_t>(1));
		Assert::IsTrue(result.at(0).getParent() == result.at(0).getID());
	}
	AdvancedQueryEngineTests& operator=(const AdvancedQueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

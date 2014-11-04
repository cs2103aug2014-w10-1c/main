//@author A0112054Y
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
#include "internal/action/update_task.h"
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

	TEST_METHOD(executeAndUndoBatchAddSubtaskQuery) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, {}, {})));
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, {}, {})));
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, {}, {})));
		auto query = QueryEngine::AddTask(
			desc, dead, prio, {}, std::move(childQueries));

		QueryEngine::executeQuery(std::move(query));

		auto graph = Internal::State::get().graph();

		auto tasklist = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));

		graph = Internal::State::get().graph();
		// TODO(evansb) define ToString
		Assert::AreEqual(tasklist.size(), static_cast<std::size_t>(1));

		QueryEngine::executeQuery(QueryEngine::Undo());

		tasklist = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));

		Assert::AreEqual(tasklist.size(), static_cast<std::size_t>(0));
	}

	TEST_METHOD(executeAndUndoBatchDeleteSubtaskQuery) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, prio, {}, {})));

		Task parent;
		{  // NOLINT
			auto query = QueryEngine::AddTask(
				desc, dead, prio, {}, std::move(childQueries));
			QueryEngine::executeQuery(std::move(query));
		}

		Assert::AreEqual(Internal::State::get().graph().getTaskCount(),
			2);

		{  // NOLINT
			auto query = QueryEngine::DeleteTask(2);
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
		auto query = QueryEngine::AddTask(
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
				{},
				{});
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
				{},
				{});
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
				{},
				{});
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
			std::move(QueryEngine::AddTask(desc, dead, Task::Priority::HIGH, {}, {})));
		QueryEngine::executeQuery(
			QueryEngine::AddTask(
			desc, dead, Task::Priority::HIGH, {}, std::move(childQueries)));
		auto getTask = QueryEngine::GetTask(Filter::highPriority());
		auto response = QueryEngine::executeQuery(std::move(getTask));
		auto result = boost::get<std::vector<Task>>(response);
		Assert::AreEqual(result.size(), static_cast<std::size_t>(1));
		Assert::IsTrue(result.at(0).isTopLevel());
	}

	TEST_METHOD(editParent) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, Task::Priority::HIGH, {}, {})));

		// Add two trees
		auto parent = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
				desc, dead, Task::Priority::HIGH, {}, std::move(childQueries))));

		std::vector<std::unique_ptr<Query>> childQueries2;
		childQueries2.push_back(
			std::move(QueryEngine::AddTask(desc, dead, Task::Priority::HIGH, {}, {})));
		auto task = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {}, std::move(childQueries2))));

		// Set the second to parent the child of first one.
		auto reparent = QueryEngine::UpdateTask(
			task.getID(), task.getDescription(), task.getDeadline(),
			task.getPriority(), task.getDependencies(),
			task.isCompleted(), parent.getID(), task.getSubtasks(),
			task.getAttachment());
		QueryEngine::executeQuery(std::move(reparent));

		auto getTask = QueryEngine::GetTask(Filter::anyTask());
		auto response = QueryEngine::executeQuery(std::move(getTask));
		auto result = boost::get<std::vector<Task>>(response);
		Assert::AreEqual(result.at(0).getSubtasks().size(), std::size_t(1));
	}

	TEST_METHOD(setToplevelWithoutChildTasksAsSubtasks) {
		// Add two trees
		auto parent = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {}, {})));

		auto parent2 = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {}, {})));

		auto parent3 = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					L"Should be shown", dead, Task::Priority::HIGH, {}, {})));

		parent3.setSubtasks({ parent.getID(), parent2.getID() });
		auto parent4 = boost::get<Task>(
			QueryEngine::executeQuery(
				std::unique_ptr<Query>(
					new Internal::Action::UpdateTask(parent3))));
		Assert::IsTrue(parent4.getSubtasks() == parent3.getSubtasks());

		auto list = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));

		Assert::AreEqual(list.size(), static_cast<std::size_t>(1));
		Assert::AreEqual(list.at(0).getDescription(),
			std::wstring(L"Should be shown"));
	}


	TEST_METHOD(setToplevelWithChildTasksAsSubtasks) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, Task::Priority::HIGH, {}, {})));

		auto parent = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {},
						std::move(childQueries))));

		auto parent2 = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {}, {})));
		parent2.setSubtasks({ parent.getID() });

		parent2 = boost::get<Task>(
			QueryEngine::executeQuery(
				std::unique_ptr<Query>(
					new Internal::Action::UpdateTask(parent2))));
		Assert::IsTrue(parent2.getSubtasks().size() == 1);
		Assert::IsTrue(parent2.getSubtasksObject().at(0).getID() == parent.getID());
	}


	TEST_METHOD(setSubtaskOfAChildTask) {
		std::vector<std::unique_ptr<Query>> childQueries;
		childQueries.push_back(
			std::move(QueryEngine::AddTask(desc, dead, Task::Priority::HIGH, {}, {})));
		auto parent = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {},
						std::move(childQueries))));
		auto child = parent.getSubtasksObject().at(0);
		auto parent2 = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {}, {})));
		child.setSubtasks({ parent2.getID() });
		child = boost::get<Task>(
			QueryEngine::executeQuery(
				std::unique_ptr<Query>(
					new Internal::Action::UpdateTask(child))));
		auto list = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));
		parent = Internal::State::get().graph().getTask(parent.getID());
		Assert::IsTrue(list.size() == 1);
		Assert::IsTrue(parent.getSubtasksObject().at(0).getID() == child.getID());
		Assert::IsTrue(child.getSubtasksObject().at(0).getID() == parent2.getID());
	}

	TEST_METHOD(deleteLevelTwoTree) {
		std::vector<std::unique_ptr<Query>> childQueries;
			childQueries.push_back(
				std::move(QueryEngine::AddTask(desc, dead, Task::Priority::HIGH, {}, {})));
		auto parent = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {},
						std::move(childQueries))));
		auto child = parent.getSubtasksObject().at(0);
		auto parent2 = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(
					desc, dead, Task::Priority::HIGH, {}, {})));
		child.setSubtasks({ parent2.getID() });
		child = boost::get<Task>(
			QueryEngine::executeQuery(
				std::unique_ptr<Query>(
					new Internal::Action::UpdateTask(child))));
		auto deleted = boost::get<Task::ID>(QueryEngine::executeQuery(
				QueryEngine::DeleteTask(child.getID())));
		Assert::IsTrue(deleted == child.getID());
		parent = Internal::State::get().graph().getTask(parent.getID());
		Assert::IsTrue(Internal::State::get().graph().getTask(parent.getID())
			.getSubtasks().empty());
	}
	AdvancedQueryEngineTests& operator=(const AdvancedQueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

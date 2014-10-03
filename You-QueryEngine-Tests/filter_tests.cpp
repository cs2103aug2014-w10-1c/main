/// \author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"
#include "internal\task_builder.h"
#include "task_model.h"
#include "api.h"
#include "filter.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using boost::gregorian::date;
using boost::gregorian::max_date_time;

using You::QueryEngine::Task;
using You::QueryEngine::Response;

using You::QueryEngine::Filter;
using You::QueryEngine::FilterTask;
using You::QueryEngine::executeQuery;

using Task = You::QueryEngine::Task;
using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;

TEST_CLASS(QueryEngineTests) {
	static const std::size_t N_TASK = 100;

	static Task getMockTask() {
		const Task::Description desc = L"Learn Haskell Lens";
		const Task::Time dead = Task::NEVER;
		const Task::Priority prio = Task::Priority::IMPORTANT;
		const Task::Dependencies dep = Task::Dependencies();
		return TaskBuilder::get().description(desc)
			.deadline(dead).priority(prio).dependencies(dep);
	}

	static State stateWithMockedTasks() {
		State state;
		for (std::size_t i = 1; i <= N_TASK; i++) {
			state.push_back(getMockTask());
		}
		return state;
	}

	TEST_METHOD(filterAnyTask) {
		State state = stateWithMockedTasks();
		using F = You::QueryEngine::Filter;
		auto result = executeQuery(FilterTask(F::anyTask()), state);

		Assert::AreEqual(boost::get<std::vector<Task>>(result).size(), N_TASK);
	}

	TEST_METHOD(filterIdIsIn) {
		State state = stateWithMockedTasks();
		std::size_t N_FILTERED = 5;
		Task::ID TEST_ID = 42L;
		std::vector<Task::ID> mustBeHere = { TEST_ID, TEST_ID + 1};
		for (int i = 1; i <= N_FILTERED; i++) {
			state.push_back(TaskBuilder::get().id(TEST_ID)
				.description(L"Dummy"));
		}
		using F = You::QueryEngine::Filter;
		auto result = executeQuery(FilterTask(F::idIsIn(mustBeHere)),
			state);

		Assert::AreEqual(boost::get<std::vector<Task>>(result).size(),
			N_FILTERED);
	}

	TEST_METHOD(logicalAndTwoFilters) {
		State state = stateWithMockedTasks();
		using F = You::QueryEngine::Filter;
		auto alwaysEmpty = F::anyTask() && (!F::anyTask());
		auto result = executeQuery(FilterTask(alwaysEmpty), state);

		Assert::AreEqual(state.size(), N_TASK);  // Must not alter state
		Assert::IsTrue(boost::get<std::vector<Task>>(result).empty());
	}

	TEST_METHOD(logicalOrTwoFilters) {
		State state = stateWithMockedTasks();
		using F = You::QueryEngine::Filter;
		auto alwaysEmpty = (!F::anyTask()) || (!F::anyTask());
		auto result = executeQuery(FilterTask(alwaysEmpty), state);

		Assert::AreEqual(state.size(), N_TASK);  // Must not alter state
		Assert::IsTrue(boost::get<std::vector<Task>>(result).empty());
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

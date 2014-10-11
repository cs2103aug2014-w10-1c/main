#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"
#include "internal/model.h"
#include "internal/controller.h"
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
using You::QueryEngine::QueryEngine;

using Task = You::QueryEngine::Task;
using State = Internal::State;
using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;

TEST_CLASS(FilterTests) {
	static const std::size_t N_TASK = 10;

	static void populateStateWithMockedTasks() {
		State::clear();
		for (std::size_t i = 1; i <= N_TASK; i++) {
			Internal::Controller::Graph::addTask(
				State::get().graph(),
				TaskBuilder::get().id(i).description(L"Clone"));
		}
	}

	TEST_METHOD(filterAnyTask) {
		populateStateWithMockedTasks();
		using F = You::QueryEngine::Filter;
		std::unique_ptr<Query> query =
			QueryEngine::GetTask(F::anyTask());
		auto result = QueryEngine::executeQuery(std::move(query));

		Assert::AreEqual(boost::get<std::vector<Task>>(result).size(), N_TASK);
	}

	TEST_METHOD(filterIdIsIn) {
		populateStateWithMockedTasks();
		std::size_t N_FILTERED = 5;
		std::vector<Task::ID> mustBeHere;
		for (std::size_t i = 1; i <= N_FILTERED; i++) {
			mustBeHere.push_back((Task::ID) i);
			Internal::Controller::Graph::addTask(
				State::get().graph(),
				TaskBuilder::get().id(i).description(L"Dummy"));
		}
		using F = You::QueryEngine::Filter;
		auto result = QueryEngine::executeQuery(
			QueryEngine::GetTask(F::idIsIn(mustBeHere)));

		Assert::AreEqual(boost::get<std::vector<Task>>(result).size(),
			N_FILTERED);
	}

	TEST_METHOD(logicalAndTwoFilters) {
		populateStateWithMockedTasks();
		using F = You::QueryEngine::Filter;
		auto alwaysEmpty = F::anyTask() && (!F::anyTask());
		auto result = QueryEngine::executeQuery(
			QueryEngine::GetTask(alwaysEmpty));

		Assert::IsTrue(boost::get<std::vector<Task>>(result).empty());
	}

	TEST_METHOD(logicalOrTwoFilters) {
		populateStateWithMockedTasks();
		using F = You::QueryEngine::Filter;
		auto alwaysEmpty = (!F::anyTask()) || (!F::anyTask());
		auto result = QueryEngine::executeQuery(
			QueryEngine::GetTask(alwaysEmpty));

		Assert::IsTrue(boost::get<std::vector<Task>>(result).empty());
	}

	TEST_METHOD(implicitConversionToLambda) {
		using F = You::QueryEngine::Filter;
		Assert::IsTrue((F::anyTask())(TaskBuilder::get().description(L"Hello")));
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

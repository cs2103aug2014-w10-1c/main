//@author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"
#include "common.h"

#include "../You-DataStore/datastore.h"
#include "internal/state.h"
#include "api.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using boost::gregorian::date;
using boost::gregorian::max_date_time;

using You::QueryEngine::Filter;
using You::QueryEngine::Task;
using You::QueryEngine::Response;
using You::QueryEngine::QueryEngine;
using You::DataStore::DataStore;

/// Test the main \ref QueryEngine API
TEST_CLASS(IntegrateWithDataStoreTest) {
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

	TEST_METHOD(initiallyDataStoreShouldBeEmpty) {
		auto taskList = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));
		Task::ID initialID = Internal::State::get().getMaxIDFromDataStore();
		Assert::AreEqual(taskList.size(), static_cast<std::size_t>(0));
		Assert::IsTrue(initialID == -1L);
	}

	TEST_METHOD(postTaskToDataStore) {
		auto task = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(desc, dead, dead, prio, {}, {})));
		Internal::State::initialize();
		Task::ID initialID = Internal::State::get().getMaxIDFromDataStore();
		Assert::IsTrue(initialID == 1L);
		auto taskList = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));
		Assert::AreEqual(taskList.size(), static_cast<std::size_t>(1));
	}

	TEST_METHOD(putTaskToDataStore) {
		auto task = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(desc, dead, dead, prio, {}, {})));

		Internal::State::initialize();
		Task::ID initialID = Internal::State::get().getMaxIDFromDataStore();
		task.setDescription(L"Hello World");
		QueryEngine::executeQuery(QueryEngine::UpdateTask(task));

		Internal::State::initialize();
		auto taskList = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));
		Assert::AreEqual(taskList[0].getDescription(),
			std::wstring(L"Hello World"));
	}

	TEST_METHOD(eraseTaskFromDataStore) {
		auto task = boost::get<Task>(
			QueryEngine::executeQuery(
				QueryEngine::AddTask(desc, dead, dead, prio, {}, {})));

		Internal::State::initialize();
		QueryEngine::executeQuery(QueryEngine::DeleteTask(task.getID()));

		Internal::State::initialize();
		auto taskList = boost::get<std::vector<Task>>(
			QueryEngine::executeQuery(
				QueryEngine::GetTask(Filter::anyTask())));
		Assert::AreEqual(taskList.size(), static_cast<std::size_t>(0));
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

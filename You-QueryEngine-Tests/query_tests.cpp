#include "stdafx.h"
#include "CppUnitTest.h"

#include "api.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTests {

using You::QueryEngine::Task;
using You::QueryEngine::AddTask;
using You::QueryEngine::GetTask;
using You::QueryEngine::DeleteTask;
using You::QueryEngine::executeQuery;

using boost::gregorian::date;
using boost::gregorian::max_date_time;

TEST_CLASS(QueryEngineTest) {
	const Task::Description desc = L"Learn Haskell Lens";
	const Task::Time dead = date(max_date_time);
	const Task::Priority prio = Task::Priority::IMPORTANT;
	const Task::Dependencies dep = Task::Dependencies();

	TEST_METHOD(constructAddTaskQuery) {
		auto resp = executeQuery(AddTask(desc, dead, prio, dep));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"ADD"));
	}

	TEST_METHOD(constructGetTaskQuery) {
		auto resp = executeQuery(GetTask(42));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"GET"));
	}

	TEST_METHOD(constructDeleteTaskQuery) {
		auto resp = executeQuery(DeleteTask(42));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"DELETE"));
	}
};
}  // namespace UnitTests

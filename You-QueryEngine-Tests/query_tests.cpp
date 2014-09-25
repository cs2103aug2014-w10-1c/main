#include "stdafx.h"
#include "CppUnitTest.h"

#include "api.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTests {

using You::QueryEngine::Task;
using You::QueryEngine::Response;
using You::QueryEngine::ADD_TASK;
using You::QueryEngine::executeQuery;

using boost::gregorian::date;
using boost::gregorian::max_date_time;

TEST_CLASS(QueryEngineTest) {
	const Task::Description desc = L"Learn Haskell Lens";
	const Task::Time dead = date(max_date_time);
	const Task::Priority prio = Task::Priority::IMPORTANT;
	const Task::Dependencies dep = Task::Dependencies();

	TEST_METHOD(constructAddTaskQuery) {
		Response resp = executeQuery(ADD_TASK(desc, dead, prio, dep));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"ADD"));
	}
};
}  // namespace UnitTests

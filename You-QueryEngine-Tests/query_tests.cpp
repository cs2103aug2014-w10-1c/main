#include "stdafx.h"
#include "CppUnitTest.h"

#include "api.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using You::QueryEngine::Task;
using You::QueryEngine::Response;
using You::QueryEngine::AddTask;
using You::QueryEngine::executeQuery;

using boost::gregorian::date;
using boost::gregorian::max_date_time;

TEST_CLASS(QueryEngineTests) {
	const Task::Description desc = L"Learn Haskell Lens";
	const Task::Time dead = Task::NEVER;
	const Task::Priority prio = Task::Priority::IMPORTANT;
	const Task::Dependencies dep = Task::Dependencies();

	TEST_METHOD(constructAddTaskQuery) {
		Response resp = executeQuery(AddTask(desc, dead, prio, dep));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"ADD"));
	}
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

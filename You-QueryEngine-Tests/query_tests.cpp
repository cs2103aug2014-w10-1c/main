#include "stdafx.h"
#include "CppUnitTest.h"

#include "query.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

using You::QueryEngine::Task;
using You::QueryEngine::AddTask;
using You::QueryEngine::GetTask;
using You::QueryEngine::DeleteTask;
using You::QueryEngine::executeQuery;

namespace UnitTests {

TEST_CLASS(QueryEngineTest) {
	/// Test whether NLP's can construct
	/// a query and it can be responded correctly
	TEST_METHOD(constructQueryForNLP) {
		const Task::Description desc = L"Learn Haskell Lens";
		const Task::Time dead = 100L;
		const Task::Priority prio = Task::Priority::IMPORTANT;
		const Task::Dependencies dep = { 1, 2, 3 };
		auto resp = executeQuery(AddTask(desc, dead, prio, dep));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"ADD"));
		resp = executeQuery(GetTask(42));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"GET"));
		resp = executeQuery(DeleteTask(42));
		Assert::AreEqual(boost::get<std::wstring>(resp),
			std::wstring(L"DELETE"));
	}
};
}  // namespace UnitTests

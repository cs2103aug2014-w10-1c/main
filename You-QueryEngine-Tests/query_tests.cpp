#include "stdafx.h"
#include "CppUnitTest.h"

#include "query.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;
namespace UnitTests {

TEST_CLASS(QueryEngineTest) {
	/// Simulate a dry run with NLP parser.
	TEST_METHOD(ParserDryRun) {
		using namespace You::QueryEngine;
		const Task::ID id = 10L;
		const Task::Description desc = L"Learn Haskell Lens";
		const Task::Time dead = 100L;
		const Task::Dependencies dep = { 1, 2, 3 };
		const Task::Priority prio = Task::Priority::IMPORTANT;
		
		auto result = executeQuery(AddTask(desc, dead, prio, dep));
		Assert::AreEqual(boost::get<std::wstring>(result),
						 std::wstring(L"ADD"));

		result = executeQuery(DeleteTask(id));
		Assert::AreEqual(boost::get<std::wstring>(result),
						 std::wstring(L"DELETE"));

		result = executeQuery(GetTask(id));
		Assert::AreEqual(boost::get<std::wstring>(result),
						 std::wstring(L"GET"));
	}
};
}  // namespace UnitTests

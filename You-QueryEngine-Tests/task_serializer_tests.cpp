
#include "stdafx.h"
#include "CppUnitTest.h"

#include "internal/task_builder.h"
#include "internal/task_serializer.h"

#include "common.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace UnitTests {

using Task = You::QueryEngine::Task;
using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;
using TaskSerializer = You::QueryEngine::Internal::TaskSerializer;

TEST_CLASS(TaskBuilderTest) {
public:
	/// Should be able to serialize a simple task.
	TEST_METHOD(serializeSimpleTask) {
		const Task::Description desc = L"Learn Haskell Lens";
		Task t = TaskBuilder::get().description(desc);
		auto serialized = TaskSerializer::serialize(t);
		Assert::AreEqual(serialized[TaskSerializer::KEY_DESCRIPTION],
			desc);
	}

	/// Should be able to deserialize a complex task.
	TEST_METHOD(serializeComplexTask) {
		const Task::Description desc = L"Learn Haskell Lens";
		using boost::posix_time::ptime;
		using boost::posix_time::time_duration;
		using boost::gregorian::date;
		using boost::gregorian::greg_month;
		const Task::Time dead = ptime(date(2002, 1, 10),
			time_duration(1, 2, 3));
		const Task::Dependencies dep = { 1, 2, 3 };
		const Task::Priority prio = Task::Priority::IMPORTANT;
		Task task = TaskBuilder::get()
			.description(desc)
			.deadline(dead)
			.priority(prio)
			.dependencies(dep);
		auto serialized = TaskSerializer::serialize(task);
		Assert::AreEqual(serialized[TaskSerializer::KEY_DESCRIPTION], desc);
		Assert::AreEqual(serialized[TaskSerializer::KEY_PRIORITY],
			std::wstring(L"important"));
		Assert::AreEqual(serialized[TaskSerializer::KEY_DEPENDENCIES],
			std::wstring(L"1;2;3;"));
		Assert::AreEqual(serialized[TaskSerializer::KEY_DEADLINE],
			std::wstring(L"2002;1;10;1;2;3;"));
	}
};
}  // namespace UnitTests

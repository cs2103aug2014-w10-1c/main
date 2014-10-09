/// \author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"

#include "internal/task_builder.h"
#include "internal/task_serializer.h"

#include "common.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::gregorian::date;
using boost::gregorian::greg_month;

using Task = You::QueryEngine::Task;
using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;
using TaskSerializer = You::QueryEngine::Internal::TaskSerializer;

TEST_CLASS(TaskBuilderTest) {
public:
	static Task getMockTask() {
		Task::Description desc = L"Learn Haskell Lens";
		Task::Time dead = ptime(date(2002, 1, 10),
			time_duration(1, 2, 3));
		Task::Dependencies dep = { 1, 2, 3 };
		Task::Priority prio = Task::Priority::HIGH;
		return TaskBuilder::get().description(desc).deadline(dead).
			priority(prio).dependencies(dep);
	}

	TEST_METHOD(taskSerializerIsUtilityClass) {
		static_assert(
		!std::is_trivially_default_constructible<TaskSerializer>::value,
		"QueryEngine cannot be default constructed");
	}

	/// Should be able to serialize a complex task.
	TEST_METHOD(serializeTask) {
		Task task = getMockTask();
		auto serialized = TaskSerializer::serialize(task);
		Assert::AreEqual(serialized[TaskSerializer::KEY_DESCRIPTION],
			task.getDescription());
		Assert::AreEqual(serialized[TaskSerializer::KEY_PRIORITY],
			std::wstring(L"HIGH"));
		Assert::AreEqual(serialized[TaskSerializer::KEY_DEPENDENCIES],
			std::wstring(L"1;2;3;"));
		Assert::AreEqual(serialized[TaskSerializer::KEY_DEADLINE],
			std::wstring(L"2002;1;10;1;2;3;"));
	}

	/// Should be able to deserialize a complex task.
	TEST_METHOD(deserializeTask) {
		Task task = getMockTask();
		auto serialized = TaskSerializer::serialize(task);
		auto deserialized = TaskSerializer::deserialize(serialized);
		Assert::AreEqual(deserialized, task);
	}
};
}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

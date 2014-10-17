#include "stdafx.h"
#include "CppUnitTest.h"
#include "internal/controller/task_builder.h"
#include "internal/exception.h"

#include "common.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

USE_WSTRING_TEST_WRITER(You::QueryEngine::Task::ID);
USE_WSTRING_TEST_WRITER(You::QueryEngine::Task::Time);

namespace You {
namespace QueryEngine {
namespace UnitTests {

/// \cond Imports
namespace {
	using Task = You::QueryEngine::Task;
	using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;
}
/// \endcond

/// Test the functionality of TaskBuilder
TEST_CLASS(TaskBuilderTests) {
	/// Should be able to create a task using minimal required.
	/// field.
	TEST_METHOD(buildValidTask) {
		const Task::Description desc = L"Learn Haskell Lens";
		Task task = TaskBuilder::get().description(desc);
		Task task2 = TaskBuilder::get().description(desc);
		Assert::AreEqual(task.getDescription(), desc);
		// They should have equal deadline, which is the default
		Assert::AreEqual(task.getDeadline(), task2.getDeadline());
	}

	/// Should be able to create a task with complete fields.
	TEST_METHOD(buildCompleteTask) {
		const Task::Description desc = L"Learn Haskell Lens";
		const Task::Time dead = Task::NEVER;
		const Task::Dependencies dep = { 1, 2, 3 };
		const Task::Priority prio = Task::Priority::HIGH;
		Task task = TaskBuilder::get()
			.description(desc)
			.deadline(dead)
			.priority(prio)
			.dependencies(dep);
		Assert::AreEqual(task.getDescription(), desc);
		// The valid one should be the last chain
		Assert::AreEqual(task.getDeadline(), dead);
		Assert::IsTrue(task.isDependOn(1));
		Assert::IsFalse(task.isDependOn(0));
	}

	/// Should throw an exception when trying to create
	/// an empty task.
	TEST_METHOD(buildEmptyDescriptionShouldThrow) {
		using You::QueryEngine::Exception::
			EmptyTaskDescriptionException;
		using TB = TaskBuilder;
		auto mustFail1 = [] { (Task) TB::get().
			deadline(Task::DEFAULT_DEADLINE); };
		auto mustFail2 = [] { (Task) TB::get().
			id(Task::DEFAULT_ID); };
		auto mustFail3 = [] { (Task) TB::get().
			dependencies(Task::DEFAULT_DEPENDENCIES); };
		auto mustFail4 = [] { (Task) TB::get().
			priority(Task::DEFAULT_PRIORITY); };
		Assert::ExpectException<EmptyTaskDescriptionException>(mustFail1);
		Assert::ExpectException<EmptyTaskDescriptionException>(mustFail2);
		Assert::ExpectException<EmptyTaskDescriptionException>(mustFail3);
		Assert::ExpectException<EmptyTaskDescriptionException>(mustFail4);
	}

	TEST_METHOD(convertTaskToString) {
		const Task::Description desc = L"Learn Haskell Lens";
		const Task::Time dead = Task::NEVER;
		const Task::Dependencies dep = { 1, 2, 3 };
		const Task::Dependencies dep2 = { 2, 1, 3};
		const Task::Priority prio = Task::Priority::HIGH;
		Task task = TaskBuilder::get()
			.description(desc)
			.deadline(dead)
			.priority(prio)
			.dependencies(dep);
		Task task2 = TaskBuilder::get()
			.description(desc)
			.deadline(dead)
			.priority(prio)
			.dependencies(dep);
		Task task3 = TaskBuilder::get()
			.description(desc)
			.deadline(dead)
			.dependencies(dep);
		Assert::AreEqual(ToString(task), ToString(task2));
		Assert::AreNotEqual(ToString(task), ToString(task3));
	}
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You


#include "stdafx.h"
#include "CppUnitTest.h"
#include "internal/task_builder.h"
#include "internal/exception.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using Task = You::QueryEngine::Task;

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

	template<>
	static std::wstring ToString<Task::ID> (const Task::ID& tid) {
		return boost::lexical_cast<std::wstring>(tid);
	}

	template<>
	static std::wstring ToString<Task::Time> (const Task::Time& time) {
		return boost::lexical_cast<std::wstring>(time);
	}

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

namespace UnitTests {

using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;

TEST_CLASS(TaskBuilderTest) {
public:
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
		const Task::Priority prio = Task::Priority::IMPORTANT;
		Task task = TaskBuilder::get()
			.description(desc)
			.deadline(dead)
			.priority(prio)
			.dependencies(dep);
		Assert::AreEqual(task.getDescription(), desc);
		// The valid one should be the last chain
		Assert::AreEqual(task.getDeadline(), dead);
	}

	/// Should throw an exception when trying to create
	/// an empty task.
	TEST_METHOD(buildEmptyDescriptionTask) {
		using You::QueryEngine::Internal::EmptyTaskDescriptionException;
		const Task::Time dead = Task::NEVER;
		Assert::ExpectException<EmptyTaskDescriptionException>([=] {
			Task task = TaskBuilder::get().deadline(dead);
		});
	}
};

}  // namespace UnitTests

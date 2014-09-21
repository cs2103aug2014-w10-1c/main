
#include "stdafx.h"
#include "CppUnitTest.h"
#include "internal/task_model.h"
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

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

namespace UnitTests {

TEST_CLASS(TaskModelTest) {
public:
	/// Should be able to create a task using minimal required.
	/// field.
	TEST_METHOD(buildValidTask) {
		const Task::Description desc = L"Learn Haskell Lens";
		Task task = Task::Builder::description(desc);
		Task task2 = Task::Builder::description(desc);
		Assert::AreEqual(task.getDescription(), desc);
		// They should have equal deadline, which is the default
		Assert::AreEqual(task.getDeadline(), task2.getDeadline());
	}

	/// Should be able to create a task with complete fields.
	TEST_METHOD(buildCompleteTask) {
		const Task::Description desc = L"Learn Haskell Lens";
		const Task::Time dead = 100L;
		const Task::Dependencies dep = { 1, 2, 3 };
		const Task::Priority prio = Task::Priority::IMPORTANT;
		Task task = Task::Builder::
			 description(desc)
			.deadline(dead)
			.priority(prio)
			.dependencies(dep)
			.deadline(dead + 1);
		Assert::AreEqual(task.getDescription(), desc);
		// The valid one should be the last chain
		Assert::AreEqual(task.getDeadline(), dead + 1);
	}

	/// Should throw an exception when trying to create
	/// an empty task.
	TEST_METHOD(buildEmptyDescriptionTask) {
		using You::QueryEngine::EmptyTaskDescriptionException;
		Assert::ExpectException<EmptyTaskDescriptionException>([] {
			Task task = Task::Builder::deadline(100L);
		});
	}
};

}  // namespace UnitTests

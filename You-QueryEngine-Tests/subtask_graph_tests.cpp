#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"
#include "mocks/task.h"
#include "internal/controller.h"
#include "internal/model.h"
#include "exception.h"
#include "api.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

/// \cond Imports
namespace {
	using Task = You::QueryEngine::Task;
	using TaskGraph = You::QueryEngine::Internal::TaskGraph;
	using Controller = You::QueryEngine::Internal::Controller;
	using State = You::QueryEngine::Internal::State;
}
/// \endcond

/// Test the functionality of subtask graph.
TEST_CLASS(SubtaskGraphTests) {
	TEST_METHOD(subtaskGraphIsCreatedOnStart) {
		Assert::IsNotNull(&State::get().sgraph());
	}

	TEST_METHOD(subtaskGraphHasTheSameTaskTableWithDependencyGraph) {
		auto list1 = State::get().sgraph().asTaskList();
		auto list2 = State::get().graph().asTaskList();
		Assert::IsTrue(std::equal(begin(list1), end(list1), begin(list2)));
	}

	TEST_METHOD(addValidTaskToSubtaskGraph) {
		TaskGraph g(TaskGraph::GraphType::SUBTASK);
		Controller::Graph::addTask(g, ONE_SUBTASK());
		Controller::Graph::addTask(g, TWO_SUBTASKS());
		Assert::AreEqual(g.getTaskCount(), 2);
	}

	TEST_METHOD(addCircularSubtaskToSubtaskGraphShouldThrowException) {
		TaskGraph g(TaskGraph::GraphType::SUBTASK);
		Assert::ExpectException<Exception::CircularDependencyException>([&g] {
			Controller::Graph::addTask(g, CIRCULAR_SUBTASKS());
		});
	}

	SubtaskGraphTests& operator=(const SubtaskGraphTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

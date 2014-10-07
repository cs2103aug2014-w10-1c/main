/// \author A0112054Y
#include "stdafx.h"
#include "CppUnitTest.h"


#include "common.h"
#include "internal/task_builder.h"
#include "internal/exception.h"
#include "api.h"

#include "internal/task_graph.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using You::QueryEngine::Task;
using You::QueryEngine::Internal::TaskGraph;
using You::QueryEngine::Internal::TaskBuilder;


TEST_CLASS(TaskGraphTests) {
	TEST_METHOD(createTaskGraph) {
		TaskGraph graph;
		Assert::AreEqual(graph.getTaskList().size(), std::size_t(0));
	}

	TEST_METHOD(addTaskToGraph) {
		TaskGraph graph;
		graph.addTask(TaskBuilder::get().id(10L).description(L"Hello World"));
		Assert::AreEqual(graph.getTaskList().size(), std::size_t(1));
		graph.addTask(TaskBuilder::get().id(11L).description(L"Hello Warld"));
		Assert::AreEqual(graph.getTaskList().size(), std::size_t(2));
		Assert::IsTrue(graph.getTaskList().at(1).getID() <= (Task::ID) 11);
	}

	TEST_METHOD(deleteExistingTaskFromGraph) {
		TaskGraph graph;
		Task task = TaskBuilder::get().id(10L).description(L"Hello Warld");
		graph.addTask(task);
		Assert::AreEqual(graph.getTaskList().size(), std::size_t(1));
		graph.deleteTask(task.getID());
		Assert::AreEqual(graph.getTaskList().size(), std::size_t(0));
	}

	TEST_METHOD(deleteNonExistingTaskFromGraph) {
		using Internal::TaskNotFoundException;
		Assert::ExpectException<TaskNotFoundException>([] {
			TaskGraph graph;
			graph.deleteTask(10L);
		});
	}

	TaskGraphTests& operator=(const TaskGraphTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

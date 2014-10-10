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


/// Test the functionality of \ref TaskGraph
TEST_CLASS(TaskGraphTests) {
using Task = You::QueryEngine::Task;
using TaskGraph = You::QueryEngine::Internal::TaskGraph;
using TaskBuilder = You::QueryEngine::Internal::TaskBuilder;

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
		using Exception::TaskNotFoundException;
		TaskGraph graph;
		Task task = TaskBuilder::get().id(10L).description(L"Hello Warld");
		graph.addTask(task);
		Assert::AreEqual(graph.getTaskList().size(), std::size_t(1));
		graph.deleteTask(task.getID());
		Assert::AreEqual(graph.getTaskList().size(), std::size_t(0));
	}

	TEST_METHOD(deleteNonExistingTaskFromGraph) {
		using Exception::TaskNotFoundException;
		Assert::ExpectException<TaskNotFoundException>([] {
			TaskGraph graph;
			graph.deleteTask(10L);
		});
	}

	TEST_METHOD(getExistingTaskFromGraph) {
		TaskGraph graph;
		Task task = TaskBuilder::get().id(10L).description(L"Hello Warld");
		graph.addTask(task);
		Assert::AreEqual(graph.getTask(task.getID()).getDescription(),
			std::wstring(L"Hello Warld"));
	}

	TEST_METHOD(getNonExistingTaskFromGraph) {
		using Exception::TaskNotFoundException;
		Assert::ExpectException<TaskNotFoundException>([=] {
			TaskGraph graph;
			graph.getTask(10L);
		});
	}

	TEST_METHOD(updateExistingTaskInGraph) {
		Task t = TaskBuilder::get().id(10L).description(L"Hello World");
		TaskGraph graph;
		graph.addTask(t);
		Task u = TaskBuilder::get().id(10L).description(L"Hello Marnie");
		graph.updateTask(u);
		Assert::AreEqual(graph.getTask(t.getID()).getDescription(),
			std::wstring(L"Hello Marnie"));
	}

	TEST_METHOD(updateNonExistingTaskInGraph) {
		using Exception::TaskNotFoundException;
		Task t = TaskBuilder::get().id(10L).description(L"Hello World");
		Assert::ExpectException<TaskNotFoundException>([=] {
			TaskGraph graph;
			graph.updateTask(t);
		});
	}

	TaskGraphTests& operator=(const TaskGraphTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

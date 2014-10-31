#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"
#include "internal/controller.h"
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
}
/// \endcond

/// Test the functionality of \ref TaskGraphController
TEST_CLASS(DependencyGraphTest) {
	TEST_METHOD(createTaskGraph) {
		TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
		Assert::AreEqual(graph.asTaskList().size(), std::size_t(0));
	}

	TEST_METHOD(addTaskToGraph) {
		TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
		Controller::Graph::addTask(graph, Controller::Builder::get()
			.id(10L).description(L"Hello World"));
		Assert::AreEqual(graph.asTaskList().size(), std::size_t(1));
		Controller::Graph::addTask(graph, Controller::Builder::get()
			.id(11L).description(L"Hello Warld"));
		Assert::AreEqual(graph.asTaskList().size(), std::size_t(2));
		Assert::IsTrue(graph.asTaskList().at(1).getID() <= (Task::ID) 11);
	}

	TEST_METHOD(deleteExistingTaskFromGraph) {
		using Exception::TaskNotFoundException;
		TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
		Task task = Controller::Builder::get().id(10L).description(L"Hello Warld");
		Controller::Graph::addTask(graph, task);
		Assert::AreEqual(graph.asTaskList().size(), std::size_t(1));
		Controller::Graph::deleteTask(graph, task.getID());
		Assert::AreEqual(graph.asTaskList().size(), std::size_t(0));
	}

	TEST_METHOD(deleteNonExistingTaskFromGraph) {
		TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
		using Exception::TaskNotFoundException;
		Assert::ExpectException<TaskNotFoundException>([] {
			TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
			Controller::Graph::deleteTask(graph, 10L);
		});
	}

	TEST_METHOD(getExistingTaskFromGraph) {
		TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
		Task task = Controller::Builder::get()
			.id(10L).description(L"Hello Warld");
		Controller::Graph::addTask(graph, task);
		Assert::AreEqual(graph.getTask(task.getID()).getDescription(),
			std::wstring(L"Hello Warld"));
	}

	TEST_METHOD(getNonExistingTaskFromGraph) {
		using Exception::TaskNotFoundException;
		Assert::ExpectException<TaskNotFoundException>([] {
			TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
			graph.getTask(10L);
		});
	}

	TEST_METHOD(updateExistingTaskInGraph) {
		Task t = Controller::Builder::get().id(10L).description(L"Hello World");
		TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
		Controller::Graph::addTask(graph, t);
		Task u = Controller::Builder::get().id(10L).description(L"Hello Marnie");
		Controller::Graph::updateTask(graph, u);
		Assert::AreEqual(graph.getTask(t.getID()).getDescription(),
			std::wstring(L"Hello Marnie"));
	}

	TEST_METHOD(updateExistingTaskInGraphThatCausesCycle) {
		TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
		Task firstTask = Controller::Builder::get()
			.id(10L).description(L"Hello World");
		Controller::Graph::addTask(graph, firstTask);

		Task secondTask = Controller::Builder::get()
			.id(11L).description(L"Hello World").dependencies({10L});
		Controller::Graph::addTask(graph, secondTask);

		Assert::AreEqual(graph.getTaskCount(), 2);
		Task willFail = Controller::Builder::get()
			.id(10L).description(L"Hello Marnie").dependencies({ secondTask.getID() });

		Assert::ExpectException<Exception::CircularDependencyException>(
			[&graph, &willFail] {
			Controller::Graph::updateTask(graph, willFail);
		});
	}

	TEST_METHOD(updateNonExistingTaskInGraph) {
		using Exception::TaskNotFoundException;
		Task t = Controller::Builder::get().id(10L).description(L"Hello World");
		Assert::ExpectException<TaskNotFoundException>([t] {
			TaskGraph graph(TaskGraph::GraphType::DEPENDENCY);
			Controller::Graph::updateTask(graph, t);
		});
	}

	// Create a dependency chain t -> t2 -> t3
	// When t2 is deleted, it should now be t -> t3
	TEST_METHOD(deleteExistingTaskFromGraphWithDependency) {
		TaskGraph g(TaskGraph::GraphType::DEPENDENCY);
		Task t = Controller::Builder::get().id(20L).description(L"test");
		Task t2 = Controller::Builder::get().id(21L).description(L"test2").
			dependencies({t.getID()});
		Task t3 = Controller::Builder::get().id(22L).description(L"test3").
			dependencies({t2.getID()});
		Controller::Graph::addTask(g, t);
		Controller::Graph::addTask(g, t2);
		Controller::Graph::addTask(g, t3);

		Assert::IsFalse(t3.isDependOn(t.getID()));
		Controller::Graph::deleteTask(g, t2.getID());
		t3 = g.getTask(t3.getID());
		Assert::IsTrue(t3.isDependOn(t.getID()));
	}

	TEST_METHOD(subtasksAndDependenciesAreSeparateConcern) {
		TaskGraph g(TaskGraph::GraphType::DEPENDENCY);
		Task t = Controller::Builder::get().id(20L).description(L"Circular")
										   .subtasks({ 20L });
		Controller::Graph::addTask(g, t);
		Assert::AreEqual(g.getTask(t.getID()), t);
	}

	DependencyGraphTest& operator=(const DependencyGraphTest&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

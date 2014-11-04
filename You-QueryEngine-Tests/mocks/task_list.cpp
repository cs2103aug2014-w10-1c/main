//@author A0112054Y
#include "stdafx.h"
#include "../You-QueryEngine/internal/controller.h"
#include "../You-QueryEngine/internal/model.h"
#include "task_list.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

namespace {
	using Controller = You::QueryEngine::Internal::Controller;
	using State = You::QueryEngine::Internal::State;
}

std::vector<Task::Description> TASK_DESCRIPTIONS() {
	return {
		std::wstring(L"CD"),
		std::wstring(L"ABC"),
		std::wstring(L"BCD"),
		std::wstring(L"ABCD"),
		std::wstring(L"EFGH"),
	};
};

std::vector<Task> ID_ONE_TO_FIVE() {
	return {
		Controller::Builder::get().description(L"meh").id(1),
		Controller::Builder::get().description(L"meh").id(2),
		Controller::Builder::get().description(L"meh").id(3),
		Controller::Builder::get().description(L"meh").id(4),
		Controller::Builder::get().description(L"meh").id(5)
	};
}

std::vector<Task> fromDescription(const std::vector<Task::Description>& v) {
	std::vector<Task> result;
	std::for_each(v.begin(), v.end(),
		[&result] (const Task::Description& d) {
			result.push_back(Controller::Builder::get().description(d));
		}
	);
	return result;
}

void populateStateWithTasks(const std::vector<Task>& tasks) {
	State::clear();
	std::for_each(tasks.begin(), tasks.end(), [] (const Task t) {
		Controller::Graph::addTask(State::get().graph(), t);
	});
}

/// @}
}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

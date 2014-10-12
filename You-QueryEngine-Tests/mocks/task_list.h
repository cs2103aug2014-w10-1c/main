#pragma once
#ifndef YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_
#define YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_

#include "../You-QueryEngine/internal/controller.h"
#include "../You-QueryEngine/internal/model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

using Task = You::QueryEngine::Task;

std::vector<Task::Description> TASK_DESCRIPTIONS = {
	std::wstring(L"CD"),
	std::wstring(L"ABC"),
	std::wstring(L"BCD"),
	std::wstring(L"ABCD"),
	std::wstring(L"EFGH"),
};

std::vector<Task> fromDescription(const std::vector<Task::Description>& v) {
	using Controller = You::QueryEngine::Internal::Controller;
	std::vector<Task> result;
	std::for_each(v.begin(), v.end(),
		[&] (const Task::Description& d) {
			result.push_back(Controller::Builder::get().description(d));
		}
	);
	return result;
}

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_

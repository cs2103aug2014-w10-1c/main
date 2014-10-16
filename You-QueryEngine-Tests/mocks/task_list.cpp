#include "stdafx.h"
#include "task_list.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

std::vector<Task::Description> TASK_DESCRIPTIONS() {
	return {
		std::wstring(L"CD"),
		std::wstring(L"ABC"),
		std::wstring(L"BCD"),
		std::wstring(L"ABCD"),
		std::wstring(L"EFGH"),
	};
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

/// @}
}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

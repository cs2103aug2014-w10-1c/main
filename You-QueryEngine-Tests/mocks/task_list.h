#pragma once
#ifndef YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_
#define YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_

#include "../You-QueryEngine/internal/controller.h"
#include "../You-QueryEngine/internal/model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

/// \name Various task lists for testing
/// @{

extern std::vector<Task::Description> TASK_DESCRIPTIONS();

extern std::vector<Task> fromDescription(
	const std::vector<Task::Description>& v);

/// @}
}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_

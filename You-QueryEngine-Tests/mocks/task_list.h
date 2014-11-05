//@author A0112054Y
#pragma once
#ifndef YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_
#define YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_

#include "../You-QueryEngine/internal/model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

/// \name Various task lists for testing
/// @{

/// List of task with random descriptions.
extern std::vector<Task::Description> TASK_DESCRIPTIONS();
/// List of tasks with ID one to five.
extern std::vector<Task> ID_ONE_TO_FIVE();

/// Helper to construct task from a list of description.
extern std::vector<Task> fromDescription(
	const std::vector<Task::Description>& v);

extern void populateStateWithTasks(
	const std::vector<Task>& tasks);

/// @}
}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TESTS_MOCKS_TASK_LIST_H_

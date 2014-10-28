#pragma once
#ifndef YOU_DATASTORE_TESTS_MOCKS_H_
#define YOU_DATASTORE_TESTS_MOCKS_H_

#include "task_typedefs.h"

namespace You {
namespace DataStore {
namespace UnitTests {

/// Dummy constants for task
/// @{
const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"description";
const std::wstring DEADLINE = L"deadline";
const std::wstring PRIORITY = L"priority";
const std::wstring DEPENDENCIES = L"dependency";
/// }@

/// Dummy serialized task
/// @{
const KeyValuePairs task1 = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
};
const KeyValuePairs task2 = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"yada yada" },
		{ DEADLINE, L"yyyyyy" },
		{ PRIORITY, L"normal" },
		{ DEPENDENCIES, L"4321" }
};
/// }@

/// Dummy constants for resources
/// @{
const std::wstring LAST_ID = L"last-id";
const std::wstring COLOUR = L"colour";
/// }@

/// Dummy constants for resources
/// @{
const KeyValuePairs res1 = {
		{ LAST_ID, L"1234" }
};
const KeyValuePairs res2 = {
		{ COLOUR, L"green" }
};
/// }@

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_TESTS_MOCKS_H_

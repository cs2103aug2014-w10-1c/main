#pragma once
#ifndef YOU_DATASTORE_TESTS_DUMMY_VALUES_H_
#define YOU_DATASTORE_TESTS_DUMMY_VALUES_H_

#include "task_typedefs.h"

namespace You {
namespace DataStore {
namespace UnitTests {

// Dummy constants
const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"description";
const std::wstring DEADLINE = L"deadline";
const std::wstring PRIORITY = L"priority";
const std::wstring DEPENDENCIES = L"dependency";

// Dummy serialized task
const SerializedTask task1 = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
};
const SerializedTask task2 = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"yada yada" },
		{ DEADLINE, L"yyyyyy" },
		{ PRIORITY, L"normal" },
		{ DEPENDENCIES, L"4321" }
};

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_TESTS_DUMMY_VALUES_H_

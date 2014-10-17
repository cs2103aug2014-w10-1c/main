#pragma once
#ifndef YOU_DATASTORE_TESTS_MOCKS_H_
#define YOU_DATASTORE_TESTS_MOCKS_H_

#include "task_typedefs.h"

namespace You {
namespace DataStore {
namespace UnitTests {

/// \name Dummy constants for testing
const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"description";
const std::wstring DEADLINE = L"deadline";
const std::wstring PRIORITY = L"priority";
const std::wstring DEPENDENCIES = L"dependency";

/// \name Dummy serialized task for testing
/// @{

extern SerializedTask task1();

extern SerializedTask task2();

/// @}

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_TESTS_MOCKS_H_

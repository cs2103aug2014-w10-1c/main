#pragma once
#ifndef YOU_DATASTORE_TESTS_MOCKS_H_
#define YOU_DATASTORE_TESTS_MOCKS_H_

#include "task_typedefs.h"

namespace You {
namespace DataStore {
namespace UnitTests {

/// \name Dummy constants for testing
extern const std::wstring TASK_ID;
extern const std::wstring DESCRIPTION;
extern const std::wstring DEADLINE;
extern const std::wstring PRIORITY;
extern const std::wstring DEPENDENCIES;

/// \name Dummy serialized task for testing
/// @{

extern SerializedTask task1();

extern SerializedTask task2();

/// @}

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_TESTS_MOCKS_H_

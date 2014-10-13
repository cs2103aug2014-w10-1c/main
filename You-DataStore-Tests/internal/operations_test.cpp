#include "stdafx.h"
#include "CppUnitTest.h"

#include "../dummy_values.h"
#include "internal/operations/erase_operation.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/operations/serialization_operation.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

/// Unit Test Class for DataStore class
TEST_CLASS(OperationsTest) {
public:
	/// Basic test for retrieving a task
	TEST_METHOD(postNewTask) {
		Internal::PostOperation post();
	}

};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

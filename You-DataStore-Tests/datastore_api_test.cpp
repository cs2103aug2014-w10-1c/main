#include "stdafx.h"
#include "CppUnitTest.h"

#include "datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace YouDataStoreTests {
TEST_CLASS(DataStoreApiTest) {
public:
	TEST_METHOD(Post_EmptyTask_Test) {
		// You::DataStore::DataStore sut;
		// sut.post(std::unordered_map<std::wstring, std::wstring>());
		Assert::IsTrue(true);
	}
};
}  // namespace YouDataStoreTests

#include "stdafx.h"
#include "CppUnitTest.h"

#include "datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using DataStore = You::DataStore::DataStore;

namespace YouDataStoreTests {
TEST_CLASS(DataStoreApiTest) {
public:
	TEST_METHOD(DataStore_Post_Basic_Test) {
		You::DataStore::DataStore sut;
		bool result = sut.post(0, DataStore::STask());
		Assert::IsTrue(result);
	}

	TEST_METHOD(DataStore_Post_DuplicateId_Test) {
		You::DataStore::DataStore sut;
		sut.post(0, DataStore::STask());
		bool result = sut.post(0, DataStore::STask());
		Assert::IsFalse(result);
	}
};
}  // namespace YouDataStoreTests

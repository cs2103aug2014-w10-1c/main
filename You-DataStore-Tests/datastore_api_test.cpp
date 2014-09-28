#include "stdafx.h"
#include "CppUnitTest.h"

#include "datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using DataStore = You::DataStore::DataStore;

namespace YouDataStoreTests {
TEST_CLASS(DataStoreApiTest) {
public:
	DataStore sut;

	TEST_METHOD(DataStore_Post_Basic_Test) {
		bool result = sut.post(0, DataStore::STask());
		Assert::IsTrue(result);
	}

	TEST_METHOD(DataStore_Post_DuplicateId_Test) {
		sut.post(0, DataStore::STask());
		bool result = sut.post(0, DataStore::STask());
		Assert::IsFalse(result);
	}

	TEST_METHOD(DataStore_Put_Basic_Test) {
		sut.post(0, DataStore::STask());
		bool result = sut.put(0, DataStore::STask());
		Assert::IsTrue(result);
	}
};
}  // namespace YouDataStoreTests

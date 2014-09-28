#include "stdafx.h"
#include "CppUnitTest.h"

#include "datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;
using DataStore = You::DataStore::DataStore;

namespace YouDataStoreTests {

const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"desc";
const std::wstring DEADLINE = L"deadl";
const std::wstring PRIORITY = L"pri";
const std::wstring DEPENDENCIES = L"depend";
const DataStore::STask stask = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
};

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

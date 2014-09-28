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
		// Assert::IsTrue(sut.document.empty());
		bool result = sut.post(0, DataStore::STask());
		Assert::IsTrue(result);
		// Assert::IsFalse(sut.document.empty());
	}

	TEST_METHOD(DataStore_Post_DuplicateId_Test) {
		sut.post(0, DataStore::STask());
		bool result = sut.post(0, DataStore::STask());
		Assert::IsFalse(result);
	}

	TEST_METHOD(DataStore_Put_Basic_Test) {
		sut.post(0, DataStore::STask());
		bool result = sut.put(0, stask);
		Assert::IsTrue(result);
		// Check for the node being edited
	}

	TEST_METHOD(DataStore_Put_FalseId_Test) {
		sut.post(0, DataStore::STask());
		bool result = sut.put(1, DataStore::STask());
		Assert::IsFalse(result);
		// Check for the node being edited
	}

	TEST_METHOD(DataStore_Get_Basic_Test) {
		sut.post(0, stask);
		boost::variant<bool, DataStore::STask> response = sut.get(0);
		DataStore::STask task = boost::get<DataStore::STask>(response);
		Assert::AreEqual(stask.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(stask.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(stask.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(stask.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(stask.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	TEST_METHOD(DataStore_Get_NonExistent_Test) {
		boost::variant<bool, DataStore::STask> response = sut.get(0);
		Assert::IsFalse(boost::get<bool>(response));
	}
};
}  // namespace YouDataStoreTests

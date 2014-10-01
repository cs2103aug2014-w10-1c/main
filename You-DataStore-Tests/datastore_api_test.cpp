#include "stdafx.h"
#include "CppUnitTest.h"

#include "datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

using DataStore = You::DataStore::DataStore;

// Dummy constants
const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"description";
const std::wstring DEADLINE = L"deadline";
const std::wstring PRIORITY = L"priority";
const std::wstring DEPENDENCIES = L"dependency";

// Dummy serialized task
const DataStore::STask stask = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
};

TEST_CLASS(DataStoreApiTest) {
public:
	TEST_METHOD(DataStore_Post_Basic_Test) {
		DataStore sut;
		// Assert::IsTrue(sut.document.empty());
		bool result = sut.post(0, DataStore::STask());
		Assert::IsTrue(result);
		// Assert::IsFalse(sut.document.empty());
	}

	TEST_METHOD(DataStore_Post_DuplicateId_Test) {
		DataStore sut;
		sut.post(0, DataStore::STask());
		bool result = sut.post(0, DataStore::STask());
		Assert::IsFalse(result);
	}

	TEST_METHOD(DataStore_Put_Basic_Test) {
		DataStore sut;
		sut.post(0, DataStore::STask());
		bool result = sut.put(0, stask);
		Assert::IsTrue(result);
		// Check for the node being edited
	}

	TEST_METHOD(DataStore_Put_FalseId_Test) {
		DataStore sut;
		sut.post(0, DataStore::STask());
		bool result = sut.put(1, DataStore::STask());
		Assert::IsFalse(result);
		// Check for the node being edited
	}

	TEST_METHOD(DataStore_Get_Basic_Test) {
		DataStore sut;
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
		DataStore sut;
		boost::variant<bool, DataStore::STask> response = sut.get(0);
		Assert::IsFalse(boost::get<bool>(response));
	}

	TEST_METHOD(DataStore_Erase_Basic_Test) {
		DataStore sut;
		sut.post(0, DataStore::STask());
		bool result = sut.erase(0);
		Assert::IsTrue(result);
	}

	TEST_METHOD(DataStore_Erase_NonExistent_Test) {
		DataStore sut;
		bool result = sut.erase(0);
		Assert::IsFalse(result);
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

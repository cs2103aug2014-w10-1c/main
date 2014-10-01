#include "stdafx.h"
#include "CppUnitTest.h"

#include "internal/internal_datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

using InternalDataStore = You::DataStore::InternalDataStore;

// Dummy constants
const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"description";
const std::wstring DEADLINE = L"deadline";
const std::wstring PRIORITY = L"priority";
const std::wstring DEPENDENCIES = L"dependency";

// Dummy serialized task
const InternalDataStore::STask stask = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
};

TEST_CLASS(DataStoreTest) {
public:
	TEST_METHOD(DataStore_Post_Basic_Test) {
		InternalDataStore sut;
		// Assert::IsTrue(sut.document.empty());
		bool result = sut.post(0, InternalDataStore::STask());
		Assert::IsTrue(result);
		// Assert::IsFalse(sut.document.empty());
	}

	TEST_METHOD(DataStore_Post_DuplicateId_Test) {
		InternalDataStore sut;
		sut.post(0, InternalDataStore::STask());
		bool result = sut.post(0, InternalDataStore::STask());
		Assert::IsFalse(result);
	}

	TEST_METHOD(DataStore_Put_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, InternalDataStore::STask());
		bool result = sut.put(0, stask);
		Assert::IsTrue(result);
		// Check for the node being edited
	}

	TEST_METHOD(DataStore_Put_FalseId_Test) {
		InternalDataStore sut;
		sut.post(0, InternalDataStore::STask());
		bool result = sut.put(1, InternalDataStore::STask());
		Assert::IsFalse(result);
		// Check for the node being edited
	}

	TEST_METHOD(DataStore_Get_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, stask);
		boost::variant<bool, InternalDataStore::STask> response = sut.get(0);
		InternalDataStore::STask task = boost::get<InternalDataStore::STask>(response);
		Assert::AreEqual(stask.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(stask.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(stask.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(stask.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(stask.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	TEST_METHOD(DataStore_Get_NonExistent_Test) {
		InternalDataStore sut;
		boost::variant<bool, InternalDataStore::STask> response = sut.get(0);
		Assert::IsFalse(boost::get<bool>(response));
	}

	TEST_METHOD(DataStore_Erase_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, InternalDataStore::STask());
		bool result = sut.erase(0);
		Assert::IsTrue(result);
	}

	TEST_METHOD(DataStore_Erase_NonExistent_Test) {
		InternalDataStore sut;
		bool result = sut.erase(0);
		Assert::IsFalse(result);
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

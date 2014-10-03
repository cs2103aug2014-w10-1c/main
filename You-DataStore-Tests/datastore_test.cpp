#include "stdafx.h"
#include "CppUnitTest.h"

#include "ds_task_typedefs.h"
#include "internal/internal_datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

using InternalDataStore = You::DataStore::Internal::InternalDataStore;

// Dummy constants
const std::wstring TASK_ID = L"id";
const std::wstring DESCRIPTION = L"description";
const std::wstring DEADLINE = L"deadline";
const std::wstring PRIORITY = L"priority";
const std::wstring DEPENDENCIES = L"dependency";

// Dummy serialized task
const STask stask = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
};

/// Unit Test Class for InternalDataStore class
TEST_CLASS(DataStoreTest) {
public:
	/// Basic test for adding a task
	TEST_METHOD(DataStore_Post_Basic_Test) {
		InternalDataStore sut;

		// Checks if the document is initially empty
		bool isEmptyDoc = sut.document.children().begin() ==
			sut.document.children().end();
		Assert::IsTrue(isEmptyDoc);

		// Checks for post return value
		bool result = sut.post(0, stask);
		Assert::IsTrue(result);

		// Updates the boolean
		isEmptyDoc = sut.document.children().begin() ==
			sut.document.children().end();
		// Checks if the document is now not empty
		Assert::IsFalse(isEmptyDoc);
	}

	/// Test for adding task with an already existing task id
	TEST_METHOD(DataStore_Post_DuplicateId_Test) {
		InternalDataStore sut;
		sut.post(0, STask());
		bool result = sut.post(0, STask());
		Assert::IsFalse(result);
	}

	/// Basic test for editing a task
	TEST_METHOD(DataStore_Put_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, STask());
		bool result = sut.put(0, stask);
		Assert::IsTrue(result);
		// Check for the node being edited
	}

	/// Test for editing task with non-existent task id
	TEST_METHOD(DataStore_Put_FalseId_Test) {
		InternalDataStore sut;
		sut.post(0, STask());
		bool result = sut.put(1, STask());
		Assert::IsFalse(result);
		// Check for the node being edited
	}

	/// Basic test for retrieving a task
	TEST_METHOD(DataStore_Get_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, stask);
		boost::variant<bool, STask> response = sut.get(0);
		STask task = boost::get<STask>(response);
		Assert::AreEqual(stask.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(stask.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(stask.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(stask.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(stask.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	/// Test for retrieving task with non-existent task id
	TEST_METHOD(DataStore_Get_NonExistent_Test) {
		InternalDataStore sut;
		boost::variant<bool, STask> response = sut.get(0);
		Assert::IsFalse(boost::get<bool>(response));
	}

	/// Basic test for erasing a task with the specified task id
	TEST_METHOD(DataStore_Erase_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, STask());
		bool result = sut.erase(0);
		Assert::IsTrue(result);
	}

	/// Test for erasing task with non-existent task id
	TEST_METHOD(DataStore_Erase_NonExistent_Test) {
		InternalDataStore sut;
		bool result = sut.erase(0);
		Assert::IsFalse(result);
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

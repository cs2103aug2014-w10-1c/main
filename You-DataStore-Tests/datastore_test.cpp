#include "stdafx.h"
#include "CppUnitTest.h"

#include "task_typedefs.h"
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
const SerializedTask stask = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"bla bla" },
		{ DEADLINE, L"xxxxxx" },
		{ PRIORITY, L"urgent" },
		{ DEPENDENCIES, L"12345" }
};
const SerializedTask staskAlt = {
		{ TASK_ID, L"0" },
		{ DESCRIPTION, L"yada yada" },
		{ DEADLINE, L"yyyyyy" },
		{ PRIORITY, L"normal" },
		{ DEPENDENCIES, L"4321" }
};

/// Unit Test Class for InternalDataStore class
TEST_CLASS(DataStoreTest) {
public:
	/// Basic test for retrieving a task
	TEST_METHOD(DataStore_Get_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, stask);
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(stask.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(stask.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(stask.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(stask.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(stask.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	/// Test for retrieving task with non-existent task id
	TEST_METHOD(DataStore_Get_NonExistent_Test) {
		InternalDataStore sut;
		boost::variant<bool, SerializedTask> response = sut.get(0);
		Assert::IsFalse(boost::get<bool>(response));
	}

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
		bool init = sut.post(0, stask);
		Assert::IsTrue(init);
		bool result = sut.post(0, SerializedTask());
		Assert::IsFalse(result);

		// Checks if the content has not changed
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(stask.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(stask.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(stask.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(stask.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(stask.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	/// Basic test for editing a task
	TEST_METHOD(DataStore_Put_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, staskAlt);
		bool result = sut.put(0, stask);
		Assert::IsTrue(result);

		// Checks if the content has changed
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(stask.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(stask.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(stask.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(stask.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(stask.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	/// Test for editing task with non-existent task id
	TEST_METHOD(DataStore_Put_FalseId_Test) {
		InternalDataStore sut;
		sut.post(0, stask);
		bool result = sut.put(1, staskAlt);
		Assert::IsFalse(result);

		// Checks if the content has not changed
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(stask.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(stask.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(stask.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(stask.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(stask.at(DEPENDENCIES), task[DEPENDENCIES]);

		// Checks if the put does not add things to the xml tree
		pugi::xpath_node_set nodeSet = sut.document.select_nodes(L"task");
		Assert::AreEqual(1, boost::lexical_cast<int>(nodeSet.size()));
	}

	/// Basic test for erasing a task with the specified task id
	TEST_METHOD(DataStore_Erase_Basic_Test) {
		InternalDataStore sut;
		sut.post(0, stask);
		// Checks if the xml tree is modified
		bool isEmptyDoc = sut.document.children().begin() ==
			sut.document.children().end();
		Assert::IsFalse(isEmptyDoc);

		bool result = sut.erase(0);
		Assert::IsTrue(result);

		// Checks if the xml tree is now empty
		isEmptyDoc = sut.document.children().begin() ==
			sut.document.children().end();
		Assert::IsTrue(isEmptyDoc);
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
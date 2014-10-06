#include "stdafx.h"
#include "CppUnitTest.h"

#include "dummy_values.h"
#include "internal/internal_datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

using InternalDataStore = You::DataStore::Internal::InternalDataStore;

/// Unit Test Class for InternalDataStore class
TEST_CLASS(DataStoreTest) {
public:
	/// Basic test for retrieving a task
	TEST_METHOD(getExistingTask) {
		InternalDataStore sut;
		sut.post(0, task1);
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(task1.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(task1.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(task1.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(task1.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(task1.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	/// Test for retrieving task with non-existent task id
	TEST_METHOD(getNonExistentTask) {
		InternalDataStore sut;
		boost::variant<bool, SerializedTask> response = sut.get(0);
		Assert::IsFalse(boost::get<bool>(response));
	}

	/// Basic test for adding a task
	TEST_METHOD(postNewTask) {
		InternalDataStore sut;

		// Checks if the document is initially empty
		bool isEmptyDoc = sut.document.children().begin() ==
			sut.document.children().end();
		Assert::IsTrue(isEmptyDoc);

		// Checks for post return value
		bool result = sut.post(0, task1);
		Assert::IsTrue(result);

		// Updates the boolean
		isEmptyDoc = sut.document.children().begin() ==
			sut.document.children().end();
		// Checks if the document is now not empty
		Assert::IsFalse(isEmptyDoc);
	}

	/// Test for adding task with an already existing task id
	TEST_METHOD(postTaskWithExistingId) {
		InternalDataStore sut;
		bool init = sut.post(0, task1);
		Assert::IsTrue(init);
		bool result = sut.post(0, SerializedTask());
		Assert::IsFalse(result);

		// Checks if the content has not changed
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(task1.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(task1.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(task1.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(task1.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(task1.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	/// Basic test for editing a task
	TEST_METHOD(putExistingTask) {
		InternalDataStore sut;
		sut.post(0, task2);
		bool result = sut.put(0, task1);
		Assert::IsTrue(result);

		// Checks if the content has changed
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(task1.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(task1.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(task1.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(task1.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(task1.at(DEPENDENCIES), task[DEPENDENCIES]);
	}

	/// Test for editing task with non-existent task id
	TEST_METHOD(putNonExistentTask) {
		InternalDataStore sut;
		sut.post(0, task1);
		bool result = sut.put(1, task2);
		Assert::IsFalse(result);

		// Checks if the content has not changed
		boost::variant<bool, SerializedTask> response = sut.get(0);
		SerializedTask task = boost::get<SerializedTask>(response);
		Assert::AreEqual(task1.at(TASK_ID), task[TASK_ID]);
		Assert::AreEqual(task1.at(DESCRIPTION), task[DESCRIPTION]);
		Assert::AreEqual(task1.at(DEADLINE), task[DEADLINE]);
		Assert::AreEqual(task1.at(PRIORITY), task[PRIORITY]);
		Assert::AreEqual(task1.at(DEPENDENCIES), task[DEPENDENCIES]);

		// Checks if the put does not add things to the xml tree
		pugi::xpath_node_set nodeSet = sut.document.select_nodes(L"task");
		Assert::AreEqual(1, boost::lexical_cast<int>(nodeSet.size()));
	}

	/// Basic test for erasing a task with the specified task id
	TEST_METHOD(eraseExistingTask) {
		InternalDataStore sut;
		sut.post(0, task1);
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
	TEST_METHOD(eraseNonExistentTask) {
		InternalDataStore sut;
		bool result = sut.erase(0);
		Assert::IsFalse(result);
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

#include "stdafx.h"
#include <CppUnitTest.h>

#include "datastore.h"
#include "transaction.h"
#include "internal/internal_datastore.h"
#include "mocks.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

/// Unit test for DataStore API that is exposed to Query Engine, namely
/// \ref DataStore and \ref Transaction
TEST_CLASS(DataStoreApiTest) {
public:
	/// Clears xml document tree and save empty xml file
	TEST_METHOD_INITIALIZE(clearDataStoreState) {
		Internal::DataStore::get().document.reset();
		Internal::DataStore::get().saveData();
	}

	TEST_METHOD_CLEANUP(cleanUpDataStoreState) {
		Internal::DataStore::get().document.reset();
		Internal::DataStore::get().saveData();
	}

	TEST_METHOD(rollbackDeleteTransactionFromStack) {
		Transaction sut(DataStore::get().begin());
		Assert::AreEqual(1U, Internal::DataStore::get().transactionStack.size());

		sut.rollback();
		Assert::AreEqual(0U, Internal::DataStore::get().transactionStack.size());
	}

	TEST_METHOD(dataStoreOperationPushedToOperationsQueue) {
		Transaction sut(DataStore::get().begin());

		DataStore::get().post(0, task1);
		Assert::AreEqual(1U, sut->operationsQueue.size());
		DataStore::get().put(0, task2);
		Assert::AreEqual(2U, sut->operationsQueue.size());
		DataStore::get().erase(0);
		Assert::AreEqual(3U, sut->operationsQueue.size());
		DataStore::get().post(LAST_ID, res2);
		Assert::AreEqual(4U, sut->operationsQueue.size());
		DataStore::get().put(LAST_ID, res1);
		Assert::AreEqual(5U, sut->operationsQueue.size());
		DataStore::get().erase(LAST_ID);
		Assert::AreEqual(6U, sut->operationsQueue.size());
	}

	TEST_METHOD(commitTransactionModifyData) {
		Transaction sut(DataStore::get().begin());
		// Operations to task
		DataStore::get().post(0, task1);
		DataStore::get().post(1, task2);
		DataStore::get().erase(0);
		// Operations to resource
		DataStore::get().post(LAST_ID, res1);
		DataStore::get().post(COLOUR, res2);
		DataStore::get().erase(LAST_ID);
		auto taskSizeBefore = DataStore::get().getAllTasks().size();
		auto resourceSizeBefore = DataStore::get().getAllResources().size();
		sut.commit();
		auto taskSizeAfter = DataStore::get().getAllTasks().size();
		auto resourceSizeAfter = DataStore::get().getAllResources().size();
		Assert::AreEqual(taskSizeBefore + 1, taskSizeAfter);
		Assert::AreEqual(resourceSizeBefore + 1, resourceSizeAfter);
	}

	TEST_METHOD(nestedTransactionExecuteOperationsInCorrectOrder) {
		Transaction sut(DataStore::get().begin());
		DataStore::get().post(0, task1);

		Transaction sut2(DataStore::get().begin());
		DataStore::get().post(1, task2);

		// Data must not be modified since the first transaction has not been
		// committed
		; {
			auto sizeBefore = DataStore::get().getAllTasks().size();
			sut2.commit();
			auto sizeAfter = DataStore::get().getAllTasks().size();
			Assert::AreEqual(sizeBefore, sizeAfter);
		}

		Transaction sut3(DataStore::get().begin());
		DataStore::get().erase(0);

		// Data must not be modified since the first transaction has not been
		// committed
		; {
			auto sizeBefore = DataStore::get().getAllTasks().size();
			sut3.commit();
			auto sizeAfter = DataStore::get().getAllTasks().size();
			// Hence, this one should be 1U
			Assert::AreEqual(sizeAfter, sizeBefore);
		}

		// Execute all the operations, with the follow order:
		// add task 0, add task 1, erase task 0
		// Therefore the end result should be sizeBefore + 1
		; {
			auto sizeBefore = DataStore::get().getAllTasks().size();
			sut.commit();
			auto sizeAfter = DataStore::get().getAllTasks().size();
			Assert::AreEqual(sizeBefore + 1, sizeAfter);
		}
	}
};

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

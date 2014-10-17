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
	static void clearDataStoreState() {
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
		clearDataStoreState();
		Transaction sut(DataStore::get().begin());

		DataStore::get().post(0, task1);
		Assert::AreEqual(1U, sut->operationsQueue.size());

		DataStore::get().put(0, task2);
		Assert::AreEqual(2U, sut->operationsQueue.size());

		DataStore::get().erase(0);
		Assert::AreEqual(3U, sut->operationsQueue.size());
	}

	TEST_METHOD(commitTransactionModifyData) {
		clearDataStoreState();
		Transaction sut(DataStore::get().begin());
		DataStore::get().post(0, task1);
		DataStore::get().post(1, task2);
		DataStore::get().erase(0);
		auto sizeBefore = DataStore::get().getAllTasks().size();
		sut.commit();
		auto sizeAfter = DataStore::get().getAllTasks().size();
		Assert::AreEqual(sizeBefore + 1, sizeAfter);
	}

	TEST_METHOD(nestedTransactionExecuteOperationsInCorrectOrder) {
		clearDataStoreState();
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

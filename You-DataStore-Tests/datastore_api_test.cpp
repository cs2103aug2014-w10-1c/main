#include "stdafx.h"
#include <CppUnitTest.h>

#include "datastore.h"
#include "transaction.h"
#include "internal/internal_datastore.h"
#include "internal/internal_transaction.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/operations/erase_operation.h"
#include "mocks.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {
TEST_CLASS(DataStoreApiTest) {
public:
	/// Clears xml document tree and save empty xml file
	static void clearDataStoreState() {
		Internal::DataStore::get().document.reset();
		Internal::DataStore::get().saveData();
	}

	TEST_METHOD(pushedOperationsAddedToTransactionOperationsQueue) {
		Transaction sut(DataStore::get().begin());

		std::unique_ptr<Internal::IOperation> postOp =
			std::make_unique<Internal::PostOperation>(0, task1);
		sut->push(std::move(postOp));
		Assert::AreEqual(1U, sut->operationsQueue.size());

		std::unique_ptr<Internal::IOperation> putOp =
			std::make_unique<Internal::PutOperation>(0, task2);
		sut->push(std::move(putOp));
		Assert::AreEqual(2U, sut->operationsQueue.size());

		std::unique_ptr<Internal::IOperation> eraseOp =
			std::make_unique<Internal::EraseOperation>(0);
		sut->push(std::move(eraseOp));
		Assert::AreEqual(3U, sut->operationsQueue.size());
	}

	TEST_METHOD(rollbackDeleteTransactionFromStack) {
		Transaction sut(DataStore::get().begin());
		Assert::AreEqual(1U, Internal::DataStore::get().transactionStack.size());

		sut.rollback();
		Assert::AreEqual(0U, Internal::DataStore::get().transactionStack.size());
	}

	TEST_METHOD(constructTransactionWithDataStoreBegin) {
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

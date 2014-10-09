#include "stdafx.h"
#include "datastore.h"
#include "transaction.h"
#include "internal/internal_transaction.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/operations/erase_operation.h"
#include "dummy_values.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {
TEST_CLASS(DataStoreApiTest) {
public:
	TEST_METHOD(pushOperationsToTransaction) {
		Transaction sut;

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

	TEST_METHOD(transactionRollback) {
		Transaction sut;

		std::unique_ptr<Internal::IOperation> postOp =
			std::make_unique<Internal::PostOperation>(0, task1);
		sut->push(std::move(postOp));

		std::unique_ptr<Internal::IOperation> putOp =
			std::make_unique<Internal::PutOperation>(0, task2);
		sut->push(std::move(putOp));

		std::unique_ptr<Internal::IOperation> eraseOp =
			std::make_unique<Internal::EraseOperation>(0);
		sut->push(std::move(eraseOp));

		Assert::AreEqual(3U, sut->operationsQueue.size());

		sut.rollback();
		Assert::AreEqual(0U, sut->operationsQueue.size());
	}

	TEST_METHOD(constructTransactionWithDataStoreBegin) {
		Transaction sut(DataStore::get().begin());

		DataStore::get().post(0, task1);
		Assert::AreEqual(1U, sut->operationsQueue.size());

		DataStore::get().put(0, task2);
		Assert::AreEqual(2U, sut->operationsQueue.size());

		DataStore::get().erase(0);
		Assert::AreEqual(3U, sut->operationsQueue.size());
		sut.commit();
	}

	TEST_METHOD(commitTransaction) {
		Transaction sut(DataStore::get().begin());
		DataStore::get().post(0, task1);
		DataStore::get().post(1, task2);
		DataStore::get().erase(0);
		sut.commit();

		int size = DataStore::get().getAllTasks().size();
		Assert::AreEqual(1, size);
	}
};

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

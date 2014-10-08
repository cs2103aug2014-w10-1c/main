#include "stdafx.h"
#include "CppUnitTest.h"

#include "datastore.h"
#include "transaction.h"
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

		std::shared_ptr<Internal::IOperation> postOp =
			std::make_shared<Internal::PostOperation>(0, task1);
		sut.push(postOp);
		Assert::AreEqual(boost::lexical_cast<size_t>(1), sut.operationsQueue.size());

		std::shared_ptr<Internal::IOperation> putOp =
			std::make_shared<Internal::PutOperation>(0, task2);
		sut.push(putOp);
		Assert::AreEqual(boost::lexical_cast<size_t>(2), sut.operationsQueue.size());

		std::shared_ptr<Internal::IOperation> eraseOp =
			std::make_shared<Internal::EraseOperation>(0);
		sut.push(eraseOp);
		Assert::AreEqual(boost::lexical_cast<size_t>(3), sut.operationsQueue.size());
	}

	TEST_METHOD(transactionRollback) {
		Transaction sut;

		std::shared_ptr<Internal::IOperation> postOp =
			std::make_shared<Internal::PostOperation>(0, task1);
		sut.push(postOp);

		std::shared_ptr<Internal::IOperation> putOp =
			std::make_shared<Internal::PutOperation>(0, task2);
		sut.push(putOp);

		std::shared_ptr<Internal::IOperation> eraseOp =
			std::make_shared<Internal::EraseOperation>(0);
		sut.push(eraseOp);

		Assert::AreEqual(boost::lexical_cast<size_t>(3), sut.operationsQueue.size());

		sut.rollback();
		Assert::AreEqual(boost::lexical_cast<size_t>(0), sut.operationsQueue.size());
	}

	TEST_METHOD(constructTransactionWithDataStoreBegin) {
		Transaction& sut(DataStore::get().begin());

		DataStore::get().post(0, task1);
		Assert::AreEqual(boost::lexical_cast<size_t>(1), sut.operationsQueue.size());

		DataStore::get().put(0, task2);
		Assert::AreEqual(boost::lexical_cast<size_t>(2), sut.operationsQueue.size());

		DataStore::get().erase(0);
		Assert::AreEqual(boost::lexical_cast<size_t>(3), sut.operationsQueue.size());
		sut.commit();
	}

	TEST_METHOD(commitTransaction) {
		Transaction& sut(DataStore::get().begin());
		DataStore::get().post(0, task1);
		DataStore::get().post(1, task2);
		DataStore::get().erase(0);
		sut.commit();
		int size = DataStore::get().getAllTask().size();
		Assert::AreEqual(1, size);
	}
};

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

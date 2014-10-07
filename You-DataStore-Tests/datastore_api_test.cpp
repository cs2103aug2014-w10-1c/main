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

};

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

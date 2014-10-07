#include "stdafx.h"
#include "CppUnitTest.h"

#include "datastore.h"
#include "transaction.h"
#include "internal/operations/post_operation.h"
#include "dummy_values.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {
TEST_CLASS(DataStoreApiTest) {
public:
	TEST_METHOD(pushOperationToTransaction) {
		Transaction sut;
		Internal::PostOperation postOp(0, task1);
		sut.push(std::shared_ptr<Internal::IOperation>(&postOp));
		Assert::AreEqual(boost::lexical_cast<size_t>(1), sut.operationsQueue.size());
	}
};

}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

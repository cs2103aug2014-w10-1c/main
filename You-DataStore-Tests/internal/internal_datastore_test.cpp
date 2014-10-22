#include "stdafx.h"
#include <CppUnitTest.h>

#include "../mocks.h"
#include "internal/operations/erase_operation.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/internal_datastore.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

using DataStore = You::DataStore::Internal::DataStore;

/// Unit Test Class for DataStore class
TEST_CLASS(DataStoreTest) {
public:
	TEST_METHOD_INITIALIZE(clearDataStoreState) {
		DataStore::get().document.reset();
		DataStore::get().saveData();
	}

	TEST_METHOD_CLEANUP(cleanUpDataStoreState) {
		DataStore::get().document.reset();
		DataStore::get().saveData();
	}

	TEST_METHOD(beginTransactionAddsToTransactionStack) {
		DataStore& sut = DataStore::get();
		Assert::IsTrue(sut.transactionStack.empty());
		Transaction t(sut.begin());
		Assert::AreEqual(1U, sut.transactionStack.size());
	}

	TEST_METHOD(pushedOperationsAddedToTransactionOperationsQueue) {
		DataStore& sut = DataStore::get();
		Transaction t(sut.begin());
		sut.post(10, task1);
		Assert::AreEqual(1U, t->operationsQueue.size());
		sut.put(10, task2);
		Assert::AreEqual(2U, t->operationsQueue.size());
		sut.erase(10);
		Assert::AreEqual(3U, t->operationsQueue.size());
	}

	TEST_METHOD(commitChangesXmlDocumentTree) {
		DataStore& sut = DataStore::get();
		Transaction t(sut.begin());
		sut.post(10, task1);

		// Note: To check if document is not changed after commit requires
		// 2 first_child()s because the first one retrieves the tasks node
		// while the second one is to check if the children of the tasks node
		// is empty

		// document must not change without commit
		Assert::IsTrue(sut.document.first_child().first_child().empty());
		t.commit();
		// document changes after commit
		Assert::IsFalse(sut.document.first_child().first_child().empty());

		Transaction t2(sut.begin());
		sut.erase(10);
		// document must not change without commit
		Assert::IsFalse(sut.document.first_child().first_child().empty());
		t2.commit();
		// document changes after commit
		Assert::IsTrue(sut.document.first_child().first_child().empty());
	}

	TEST_METHOD(rollbackDeleteTransactionFromStack) {
		DataStore& sut = DataStore::get();
		Transaction t(sut.begin());
		Assert::AreEqual(1U, sut.transactionStack.size());
		t.rollback();
		Assert::AreEqual(0U, sut.transactionStack.size());
	}

	TEST_METHOD(getAllTasksFromTree) {
		DataStore& sut = DataStore::get();

		// Create mock
		sut.document.append_child(L"tasks").append_child(L"task").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"what");

		std::vector<KeyValuePairs> result = sut.getAllTask();
		Assert::AreEqual(1U, result.size());
	}

	TEST_METHOD(getAllTaskFromFile) {
		DataStore& sut = DataStore::get();

		// Create mock
		sut.document.append_child(L"tasks").append_child(L"task").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"what");
		sut.document.save_file(sut.FILE_PATH.c_str());
		sut.document.reset();

		std::vector<KeyValuePairs> result = sut.getAllTask();
		Assert::AreEqual(1U, result.size());
	}

	TEST_METHOD(saveAndLoadTheSameThing) {
		DataStore& sut = DataStore::get();

		sut.document.append_child(L"task").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"what");
		bool result = sut.saveData();
		Assert::IsTrue(result);

		sut.document.reset();

		sut.loadData();
		std::wstring value = sut.document.child(L"task").child_value();
		Assert::AreEqual(std::wstring(L"what"), value);
	}

	TEST_METHOD(pushOperationToTransactionWithoutDataStore) {
		Internal::Transaction sut;

		std::unique_ptr<Internal::IOperation> post =
			std::make_unique<Internal::PostOperation>(0, task1);
		sut.push(std::move(post));
		Assert::AreEqual(1U, sut.operationsQueue.size());

		std::unique_ptr<Internal::IOperation> put =
			std::make_unique<Internal::PutOperation>(0, task1);
		sut.push(std::move(put));
		Assert::AreEqual(2U, sut.operationsQueue.size());

		std::unique_ptr<Internal::IOperation> erase =
			std::make_unique<Internal::EraseOperation>(0);
		sut.push(std::move(erase));
		Assert::AreEqual(3U, sut.operationsQueue.size());

		sut.operationsQueue.clear();
	}

	TEST_METHOD(mergeOperationsQueueIsAppend) {
		boost::ptr_deque<Internal::IOperation> q1;
		boost::ptr_deque<Internal::IOperation> q2;

		std::unique_ptr<Internal::IOperation> post =
			std::make_unique<Internal::PostOperation>(0, task1);
		std::unique_ptr<Internal::IOperation> erase =
			std::make_unique<Internal::EraseOperation>(0);
		q1.push_back(post.release());
		q2.push_back(erase.release());

		Internal::Transaction sut;
		sut.mergeOperationsQueue(q1);
		Assert::AreEqual(1U, sut.mergedOperationsQueue.size());
		sut.mergeOperationsQueue(q2);
		Assert::AreEqual(2U, sut.mergedOperationsQueue.size());

		pugi::xml_document mockDocument;

		// Check if the order of operation is correct
		bool result = sut.mergedOperationsQueue.front().run(mockDocument);
		Assert::IsTrue(result);
		result = sut.mergedOperationsQueue.back().run(mockDocument);
		Assert::IsTrue(result);
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

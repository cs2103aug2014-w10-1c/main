//@author A0114171W
#include "stdafx.h"
#include <CppUnitTest.h>

#include <cstdio>
#include <fstream>
#include "../mocks.h"
#include "internal/operations/erase_operation.h"
#include "internal/operations/post_operation.h"
#include "internal/operations/put_operation.h"
#include "internal/internal_datastore.h"
#include "internal/constants.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace DataStore {
namespace UnitTests {

using DataStore = You::DataStore::Internal::DataStore;

/// Unit Test Class for \ref Internal::DataStore class
TEST_CLASS(DataStoreTest) {
public:
	TEST_METHOD_INITIALIZE(clearDataStoreState) {
		DataStore::get().document.reset();
		std::remove("data.xml");
	}

	TEST_METHOD_CLEANUP(cleanUpDataStoreState) {
		DataStore::get().document.reset();
		std::remove("data.xml");
	}

	/// Checks if DataStore::get() method adds a new transaction into
	/// the active transaction stack
	TEST_METHOD(beginTransactionAddsToTransactionStack) {
		DataStore& sut = DataStore::get();
		Assert::IsTrue(sut.transactionStack.empty());
		Transaction t(sut.begin());
		Assert::AreEqual(1U, sut.transactionStack.size());
	}

	/// Checks if post, put, and erase methods add \ref Internal::Operation
	/// objects into the active transaction's operationsQueue
	TEST_METHOD(pushedOperationsAddedToTransactionOperationsQueue) {
		DataStore& sut = DataStore::get();
		Transaction t(sut.begin());
		sut.post(Internal::TASKS_NODE, L"10", task1);
		Assert::AreEqual(1U, t->operationsQueue.size());
		sut.put(Internal::TASKS_NODE, L"10", task2);
		Assert::AreEqual(2U, t->operationsQueue.size());
		sut.erase(Internal::TASKS_NODE, L"10");
		Assert::AreEqual(3U, t->operationsQueue.size());
	}

	/// Checks if the document is only changed when a transaction is committed
	TEST_METHOD(commitChangesXmlDocumentTree) {
		DataStore& sut = DataStore::get();
		Transaction t(sut.begin());
		sut.post(Internal::TASKS_NODE, L"10", task1);

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
		sut.erase(Internal::TASKS_NODE, L"10");
		// document must not change without commit
		Assert::IsFalse(sut.document.first_child().first_child().empty());
		t2.commit();
		// document changes after commit
		Assert::IsTrue(sut.document.first_child().first_child().empty());
	}

	/// Checks if rollback cleans up the transaction stack too
	TEST_METHOD(rollbackDeleteTransactionFromStack) {
		DataStore& sut = DataStore::get();
		Transaction t(sut.begin());
		Assert::AreEqual(1U, sut.transactionStack.size());
		t.rollback();
		Assert::AreEqual(0U, sut.transactionStack.size());
	}

	/// Unit test to check if getAll behaves correctly when getting from the
	/// XML document tree
	TEST_METHOD(getAllFromTree) {
		DataStore& sut = DataStore::get();

		// Create mock
		sut.document.append_child(L"tasks").append_child(L"task").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"what");

		std::vector<KeyValuePairs> result = sut.getAll(L"tasks");
		Assert::AreEqual(1U, result.size());
	}

	/// Unit test to check if getAll behaves correctly when getting from
	/// file (scenario: during load)
	TEST_METHOD(getAllFromFile) {
		DataStore& sut = DataStore::get();

		// Create mock
		sut.document.append_child(L"tasks").append_child(L"task").
			append_child(pugi::xml_node_type::node_pcdata).set_value(L"what");
		sut.document.save_file(sut.FILE_PATH.c_str());
		sut.document.reset();

		std::vector<KeyValuePairs> result = sut.getAll(L"tasks");
		Assert::AreEqual(1U, result.size());
	}

	/// Checks if saving and loading works
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

	/// Unit test for \ref Internal::Transaction 's push operation
	TEST_METHOD(pushOperationToTransactionWithoutDataStore) {
		Internal::Transaction sut;

		std::unique_ptr<Internal::Operation> post =
			std::make_unique<Internal::PostOperation>(Internal::TASKS_NODE, L"0", task1);
		sut.push(std::move(post));
		Assert::AreEqual(1U, sut.operationsQueue.size());

		std::unique_ptr<Internal::Operation> put =
			std::make_unique<Internal::PutOperation>(Internal::TASKS_NODE, L"0", task1);
		sut.push(std::move(put));
		Assert::AreEqual(2U, sut.operationsQueue.size());

		std::unique_ptr<Internal::Operation> erase =
			std::make_unique<Internal::EraseOperation>(Internal::TASKS_NODE, L"0");
		sut.push(std::move(erase));
		Assert::AreEqual(3U, sut.operationsQueue.size());

		sut.operationsQueue.clear();
	}

	/// Unit test for \ref Internal::Transaction 's mergeOperationsQueue method
	TEST_METHOD(mergeOperationsQueueIsAppend) {
		boost::ptr_deque<Internal::Operation> q1;
		boost::ptr_deque<Internal::Operation> q2;

		std::unique_ptr<Internal::Operation> post =
			std::make_unique<Internal::PostOperation>(Internal::TASKS_NODE, L"0", task1);
		std::unique_ptr<Internal::Operation> erase =
			std::make_unique<Internal::EraseOperation>(Internal::TASKS_NODE, L"0");
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

	TEST_METHOD(wipeData) {
		DataStore::get().wipeData();
	}
};
}  // namespace UnitTests
}  // namespace DataStore
}  // namespace You

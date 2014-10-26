#pragma once
#ifndef YOU_DATASTORE_INTERNAL_INTERNAL_TRANSACTION_H_
#define YOU_DATASTORE_INTERNAL_INTERNAL_TRANSACTION_H_

#include <memory>
#include <boost/ptr_container/ptr_deque.hpp>

#include "operation.h"

namespace You {
namespace DataStore {
namespace UnitTests { class DataStoreTest; class DataStoreApiTest; }
namespace Internal {

/// The actual class that contains the logic for managing transactions.
class Transaction {
	friend class DataStore;
	friend class UnitTests::DataStoreTest;
	friend class UnitTests::DataStoreApiTest;

public:
	/// Default constructor. This is meant to be called by \ref DataStore.
	Transaction() = default;

	/// Commits the set of operations made.
	void commit();

	/// Rolls back all the operations made.
	void rollback();

	/// Pushes a transaction onto the stack. This is meant to be called by
	/// \ref DataStore.
	///
	/// \param[in] operation The operation to push.
	void push(std::unique_ptr<Operation> operation);

	/// Merges the operationsQueue of the next transaction that is committed
	/// earlier.
	///
	/// \param[in] queue The operations queue
	void mergeOperationsQueue(boost::ptr_deque<Operation>& queue);

	bool operator==(Transaction&);

private:
	/// The set of operations that need to be executed when the transaction is
	/// committed.
	boost::ptr_deque<Operation> operationsQueue;

	boost::ptr_deque<Operation> mergedOperationsQueue;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_INTERNAL_TRANSACTION_H_

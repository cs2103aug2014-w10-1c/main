//@author A0114171W
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

/// The class that stores a group of \ref Operation s that need to collectively
/// succeed or fail
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
	/// \param[out] operation The operation to push, wrapped in a unique_ptr.
	///						  The pointer will be invalidated at the end of
	///						  the function call.
	void push(std::unique_ptr<Operation> operation);

	/// Merges the operationsQueue of the next transaction that is committed
	/// earlier.
	///
	/// \param[out] queue The operations queue
	///					  The queue will be empty at the end of the function call.
	void mergeOperationsQueue(boost::ptr_deque<Operation>& queue);

	/// Checks whether this and rhs refer the same \ref Internal::Transaction
	///
	/// \param[in] rhs The other Transaction to compare with
	bool operator==(Transaction& rhs);

private:
	/// The collection of operations that need to be executed when the
	/// transaction is committed.
	boost::ptr_deque<Operation> operationsQueue;

	/// The collection of operations from the subsequent commits that are
	/// merged before this transaction.
	boost::ptr_deque<Operation> mergedOperationsQueue;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_INTERNAL_TRANSACTION_H_

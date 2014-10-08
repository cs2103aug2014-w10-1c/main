#pragma once
#ifndef YOU_DATASTORE_TRANSACTION_H_
#define YOU_DATASTORE_TRANSACTION_H_

#include <memory>

namespace You {
namespace DataStore {
namespace UnitTests { class DataStoreApiTest; }
namespace Internal { class InternalDataStore; class Transaction; }

/// Represents a collection of operations that need to collectively succeed or
/// fail.
///
/// This is a RAII class. At the end of the method, if \ref commit is not
/// called, the transaction will be rolled back.
class Transaction : protected std::shared_ptr<Internal::Transaction> {
	friend class Internal::InternalDataStore;
	friend class UnitTests::DataStoreApiTest;

public:
	/// Move constructor. We only allow a transaction to have one strong
	/// reference.
	Transaction(Transaction&& transaction);

	/// Destructor. Rolls back the active transaction if it has not been
	/// committed.
	~Transaction();

	/// Commits the set of operations made.
	void commit();

	/// Rolls back all the operations made.
	void rollback();

private:
	/// The state of the transaction.
	enum class State {
		/// The transaction has neither been committed nor rolled back.
		NEITHER = 0,

		/// The transaction has been committed.
		COMMITTED,

		/// The transaction has been rolled back.
		ROLLED_BACK
	};

private:
	/// Default constructor. This is meant to be called by \ref DataStore.
	Transaction();

	/// Disable the copy constructor.
	Transaction(const Transaction&) = delete;

	/// Disable the copy assignment.
	Transaction& operator=(const Transaction&) = delete;

private:
	/// The state of the transaction.
	State state;
};

}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_TRANSACTION_H_

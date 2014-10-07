#pragma once
#ifndef YOU_DATASTORE_TRANSACTION_H_
#define YOU_DATASTORE_TRANSACTION_H_

namespace You {
namespace DataStore {
class DataStore;
class Transaction {
	friend class DataStore;
public:
	Transaction(Transaction&&);
	void commit();
	void rollback();

private:
	Transaction();
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_TRANSACTION_H_

#pragma once
#ifndef YOU_DATASTORE_TRANSACTION_H_
#define YOU_DATASTORE_TRANSACTION_H_

namespace You {
namespace DataStore {
class Transaction {
public:
	Transaction();
	Transaction(Transaction&&);
	void commit();
	void rollback();
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_TRANSACTION_H_

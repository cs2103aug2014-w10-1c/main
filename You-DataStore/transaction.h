#pragma once
#ifndef YOU_DATASTORE_TRANSACTION_H_
#define YOU_DATASTORE_TRANSACTION_H_

#include "internal/operation.h"

namespace You {
namespace DataStore {
class DataStore;
class Transaction {
	friend class DataStore;
public:
	Transaction(Transaction&&);
	void commit();
	void rollback();
	void push(Internal::IOperation&);

private:
	Transaction();
	std::deque<Internal::IOperation> operationsQueue;
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_TRANSACTION_H_

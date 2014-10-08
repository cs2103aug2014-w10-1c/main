#pragma once
#ifndef YOU_DATASTORE_TRANSACTION_H_
#define YOU_DATASTORE_TRANSACTION_H_

#include <deque>
#include <memory>

namespace You {
namespace DataStore {
namespace UnitTests { class DataStoreApiTest; }
namespace Internal { class IOperation; }

class DataStore;
class Transaction {
	friend class DataStore;
	friend class UnitTests::DataStoreApiTest;

public:
	Transaction(Transaction&);
	void commit();
	void rollback();

private:
	Transaction();
	void push(std::shared_ptr<Internal::IOperation>);

private:
	std::deque<std::shared_ptr<Internal::IOperation>> operationsQueue;
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_TRANSACTION_H_

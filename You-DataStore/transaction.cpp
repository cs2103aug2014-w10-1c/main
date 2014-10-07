#include "stdafx.h"
#include "datastore.h"
#include "transaction.h"

namespace You {
namespace DataStore {

Transaction::Transaction(Transaction&& t) {
	*this = std::move(t);
}

void Transaction::commit() {
	for (auto iter = operationsQueue.begin(); iter != operationsQueue.end();
		++iter) {
		bool success = (*iter)->run();
		if (!success) {
			return rollback();
		}
	}
}

void Transaction::rollback() {
	operationsQueue.clear();
}

void Transaction::push(std::shared_ptr<Internal::IOperation> op) {
	operationsQueue.push_back(op);
}

Transaction::Transaction() {}

}  // namespace DataStore
}  // namespace You

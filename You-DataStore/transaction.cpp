#include "stdafx.h"
#include "datastore.h"
#include "transaction.h"

namespace You {
namespace DataStore {

Transaction::Transaction(Transaction&& t) {
	*this = std::move(t);
}

void Transaction::commit() {
	while (!operationsQueue.empty()) {
		Internal::IOperation& op = operationsQueue.front();
		bool success = op.run();
		if (!success) {
			return rollback();
		}
		operationsQueue.pop_front();
	}
}

void Transaction::rollback() {
	operationsQueue.clear();
}

void Transaction::push(Internal::IOperation& op) {
	operationsQueue.push_back(op);
}

Transaction::Transaction() {}

}  // namespace DataStore
}  // namespace You

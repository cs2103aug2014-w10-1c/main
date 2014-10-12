#include "stdafx.h"
#include "internal_datastore.h"
#include "internal_transaction.h"

namespace You {
namespace DataStore {
namespace Internal {

void Transaction::commit() {
	DataStore::get().onTransactionCommit(*this);
}

void Transaction::rollback() {
	DataStore::get().onTransactionRollback(*this);
}

void Transaction::push(std::unique_ptr<Internal::IOperation> op) {
	operationsQueue.push_back(op.release());
}

bool Transaction::operator==(Transaction& other) {
	return &*this == &other;
}

void Transaction::mergeOperationsQueue(boost::ptr_deque<IOperation>& queue) {
	mergedOperationsQueue.transfer(mergedOperationsQueue.end(), queue.begin(),
		queue.end(), queue);
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

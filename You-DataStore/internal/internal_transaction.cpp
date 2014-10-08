#include "stdafx.h"
#include "internal_transaction.h"

namespace You {
namespace DataStore {
namespace Internal {

void Transaction::commit() {
#if 0
	for (auto iter = operationsQueue.begin(); iter != operationsQueue.end();
		++iter) {
		bool success = (*iter)->run();
		if (!success) {
			return rollback();
		}
	}
	DataStore::get().notifyCommit();
#endif
}

void Transaction::rollback() {
#if 0
	operationsQueue.clear();
	DataStore::get().notifyRollback();
#endif
}

void Transaction::push(std::unique_ptr<Internal::IOperation> op) {
	operationsQueue.push_back(op.release());
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

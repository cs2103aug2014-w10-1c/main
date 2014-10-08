#include "stdafx.h"
#include "internal/internal_transaction.h"
#include "transaction.h"

namespace You {
namespace DataStore {

Transaction::Transaction()
: std::shared_ptr<Internal::Transaction>(
std::make_shared<Internal::Transaction>()) {
}

Transaction::Transaction(Transaction&& t) {
	std::shared_ptr<Internal::Transaction>::operator=(std::move(t));
}

void Transaction::commit() {
	operator->()->commit();
}

void Transaction::rollback() {
	operator->()->rollback();
}

}  // namespace DataStore
}  // namespace You

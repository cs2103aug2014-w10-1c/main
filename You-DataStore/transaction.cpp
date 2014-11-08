//@author A0097630B
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
	state = t.state;
	std::shared_ptr<Internal::Transaction>::operator=(std::move(t));
}

Transaction::~Transaction() {
	if (get() != nullptr && state == State::NEITHER) {
		rollback();
	}
}

void Transaction::commit() {
	assert(state == State::NEITHER);

	operator->()->commit();
	state = State::COMMITTED;
}

void Transaction::rollback() {
	assert(state == State::NEITHER);

	operator->()->rollback();
	state = State::ROLLED_BACK;
}

}  // namespace DataStore
}  // namespace You

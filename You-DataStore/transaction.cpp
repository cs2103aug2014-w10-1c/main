#include "stdafx.h"
#include "datastore.h"
#include "transaction.h"

namespace You {
namespace DataStore {

Transaction::Transaction() { }

Transaction::Transaction(Transaction&& t) {
	*this = std::move(t);
}

}  // namespace DataStore
}  // namespace You

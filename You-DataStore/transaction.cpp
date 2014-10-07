#include "stdafx.h"
#include "datastore.h"
#include "transaction.h"

namespace You {
namespace DataStore {

Transaction::Transaction(Transaction&& t) {
	*this = std::move(t);
}

Transaction::Transaction() {}

}  // namespace DataStore
}  // namespace You

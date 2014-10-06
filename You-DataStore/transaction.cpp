#include "stdafx.h"
#include "datastore.h"
#include "transaction.h"

namespace You {
namespace DataStore {

Transaction::Transaction(bool busy) {
	// while (DataStore::begin()) {}  // for multithreading
	DataStore::get().isServing = true;
}

}  // namespace DataStore
}  // namespace You

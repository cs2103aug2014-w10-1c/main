#include "stdafx.h"
#include "datastore.h"
#include "transaction.h"

namespace You {
namespace DataStore {

Transaction::Transaction(bool busy) {
	while (busy) {
		busy = DataStore::begin();
	}
	DataStore::getInstance().isServing = true;
}

}  // namespace DataStore
}  // namespace You

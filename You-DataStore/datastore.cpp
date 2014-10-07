#include "stdafx.h"
#include "datastore.h"

namespace You {
namespace DataStore {

DataStore& DataStore::get() {
	static DataStore ds;
	return ds;
}

Transaction& DataStore::begin() {
	while (this->isServing) { }
	isServing = true;
	transactionStack.push(std::shared_ptr<Transaction>(new Transaction()));
	return *(transactionStack.top());
}

void DataStore::notify() {
	isServing = false;
}

}  // namespace DataStore
}  // namespace You

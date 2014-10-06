#include "stdafx.h"
#include "datastore.h"

namespace You {
namespace DataStore {

DataStore& DataStore::getInstance() {
	static DataStore ds;
	return ds;
}

bool DataStore::begin() {
	return getInstance().isServing;
}

}  // namespace DataStore
}  // namespace You

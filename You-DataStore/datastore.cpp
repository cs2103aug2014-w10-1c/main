#include "stdafx.h"
#include "datastore.h"

namespace You {
namespace DataStore {

DataStore& DataStore::get() {
	static DataStore ds;
	return ds;
}

bool DataStore::begin() {
	return get().isServing;
}

}  // namespace DataStore
}  // namespace You

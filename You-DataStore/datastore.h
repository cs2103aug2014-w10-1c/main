#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <deque>
#include <functional>
#include "boost/variant.hpp"
#include "task_typedefs.h"
#include "internal/operation.h"
#include "transaction.h"

namespace You {
namespace DataStore {
namespace UnitTests { class DataStoreApiTest; }

class DataStore {
	friend class UnitTests::DataStoreApiTest;
public:
	Transaction && begin();

	// Modifying methods
	bool post(TaskId, SerializedTask&);
	bool put(TaskId, SerializedTask&);
	bool erase(TaskId);

	std::vector<SerializedTask> getAllTask();

private:
	static DataStore& get();
	bool isServing = false;
	std::deque<Internal::IOperation> operationsQueue;
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_DATASTORE_H_

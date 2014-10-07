#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <memory>
#include "task_typedefs.h"
#include "transaction.h"

namespace You {
namespace DataStore {
namespace UnitTests {}

class DataStore {
public:
	Transaction && begin();

	static DataStore& get();

	// Modifying methods
	void post(TaskId, SerializedTask&);
	void put(TaskId, SerializedTask&);
	void erase(TaskId);

	std::vector<SerializedTask> getAllTask();

private:
	bool isServing = false;
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_DATASTORE_H_

#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <stack>
#include <memory>
#include "task_typedefs.h"
#include "transaction.h"

namespace You {
namespace DataStore {
namespace UnitTests {}

class DataStore {
public:
	Transaction& begin();

	static DataStore& get();

	// Modifying methods
	void post(TaskId, const SerializedTask&);
	void put(TaskId, const SerializedTask&);
	void erase(TaskId);

	std::vector<SerializedTask> getAllTask();

	void notify();

private:
	bool isServing = false;
	std::shared_ptr<Transaction> activeTransaction;
	std::stack<std::shared_ptr<Transaction>> transactionStack;
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_DATASTORE_H_

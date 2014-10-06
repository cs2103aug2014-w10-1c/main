#pragma once
#ifndef YOU_DATASTORE_DATASTORE_H_
#define YOU_DATASTORE_DATASTORE_H_

#include <deque>
#include <functional>
#include "boost/variant.hpp"
#include "task_typedefs.h"
#include "internal/operation.h"

namespace You {
namespace DataStore {
namespace UnitTests {}
class Transaction;
class DataStore {
	friend class Transaction;
public:
	static bool begin();

	// Modifying methods
	static bool post(TaskId, SerializedTask&);
	static bool put(TaskId, SerializedTask&);
	static bool erase(TaskId);

	static boost::variant<bool, SerializedTask> get(TaskId);
	static std::vector<SerializedTask>
		filter(const std::function<bool(SerializedTask)>&);
private:
	static DataStore& getInstance();
	bool isServing = false;
	std::deque<Internal::IOperation> operationsQueue;
};
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_DATASTORE_H_

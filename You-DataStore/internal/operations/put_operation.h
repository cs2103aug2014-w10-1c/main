#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

#include "serialization_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

class PutOperation : public SerializationOperation {
public:
	PutOperation(TaskId, const KeyValuePairs&);
	bool run(pugi::xml_node& document) override;
	virtual ~PutOperation() = default;

private:
	/// The new task contents.
	KeyValuePairs task;
};
}  // namespace Internal
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

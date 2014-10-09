#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

#include "serialization_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

class PutOperation : public SerializationOperation {
public:
	PutOperation(TaskId, const SerializedTask&);
	bool run(pugi::xml_document& document) override;

private:
	/// The new task contents.
	SerializedTask task;
};
}  // namespace Internal
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

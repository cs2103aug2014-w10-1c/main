#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_

#include "serialization_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

class PostOperation : public SerializationOperation {
public:
	PostOperation(std::wstring id, const KeyValuePairs& kvp);
	bool run(pugi::xml_node& document) override;
	virtual ~PostOperation() = default;

private:
	/// The new task contents.
	KeyValuePairs task;
};
}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_

#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_SERIALIZATION_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_SERIALIZATION_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {

/// Intermediate parent class of \ref Operation requiring serialization
class SerializationOperation : public IOperation {
public:
	/// Serialize task to an xml node
	static void serialize(const KeyValuePairs&, pugi::xml_node&);

	/// Deserialize task from an xml node
	static KeyValuePairs deserialize(const pugi::xml_node&);
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_SERIALIZATION_OPERATION_H_

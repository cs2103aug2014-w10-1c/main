//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_SERIALIZATION_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_SERIALIZATION_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {

/// Intermediate parent class of \ref Operation requiring serialization
class SerializationOperation : public Operation {
public:
	/// Serialize task to an xml node
	///
	/// \param[in] kvp The KeyValuePairs to be serialized into the node
	/// \param[out] node The xml_node to be modifed
	static void serialize(const KeyValuePairs& kvp, pugi::xml_node& node);

	/// Deserialize task from an xml node
	///
	/// \param[in] node The xml_node to be deserialized
	/// \return KeyValuePairs deserialized from node
	static KeyValuePairs deserialize(const pugi::xml_node& node);
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_SERIALIZATION_OPERATION_H_

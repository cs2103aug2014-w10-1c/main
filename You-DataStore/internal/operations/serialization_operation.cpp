#include "stdafx.h"
#include "serialization_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

void SerializationOperation::serialize(const SerializedTask& stask,
	pugi::xml_node& node) {
	for (auto iter = stask.begin(); iter != stask.end(); ++iter) {
		pugi::xml_node keyNode =
			node.append_child(iter->first.c_str());
		pugi::xml_node keyValue =
			keyNode.append_child(pugi::xml_node_type::node_pcdata);
		keyValue.set_value(iter->second.c_str());
	}
}

SerializedTask SerializationOperation::deserialize(
	const pugi::xml_node& taskNode) {
	SerializedTask stask;
	for (auto iter = taskNode.begin(); iter != taskNode.end(); ++iter) {
		stask.insert(KeyValuePair(Key(iter->name()),
			Value(iter->child_value())));
	}
	return stask;
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

//@author A0114171W
#include "stdafx.h"
#include "serialization_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

void SerializationOperation::serialize(const KeyValuePairs& kvp,
	pugi::xml_node& node) {
	for (auto iter = kvp.begin(); iter != kvp.end(); ++iter) {
		pugi::xml_node keyNode =
			node.append_child(iter->first.c_str());
		pugi::xml_node keyValue =
			keyNode.append_child(pugi::xml_node_type::node_pcdata);
		keyValue.set_value(iter->second.c_str());
	}
}

KeyValuePairs SerializationOperation::deserialize(const pugi::xml_node& node) {
	KeyValuePairs kvp;
	for (auto iter = taskNode.begin(); iter != taskNode.end(); ++iter) {
		kvp.insert(KeyValuePair(Key(iter->name()),
			Value(iter->child_value())));
	}
	return kvp;
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_SAVE_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_SAVE_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {

class SerializationOperation : public IOperation {
public:
	/// Serialize task to an xml node
	static void serialize(const SerializedTask&, pugi::xml_node&);

	/// Deserialize task from an xml node
	static SerializedTask deserialize(const pugi::xml_node&);

private:
	/// The new task contents.
	SerializedTask task;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_SAVE_OPERATION_H_

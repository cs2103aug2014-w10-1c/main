//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

#include "serialization_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

/// An \ref Operation class for put operation
/// Edits the XML when \ref run is called
class PutOperation : public SerializationOperation {
public:
	/// Constructor
	///
	/// \param[in] branch the name of the branch to modify
	/// \param[in] id the id attribute of the node to be edited
	/// \param[in] kvp the KeyValuePairs to be written over the old one
	PutOperation(std::wstring branch, std::wstring id, const KeyValuePairs& kvp);
	bool run(pugi::xml_node& node) override;
	virtual ~PutOperation() = default;

private:
	/// The new task contents.
	KeyValuePairs task;
};
}  // namespace Internal
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_PUT_OPERATION_H_

#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_

#include "serialization_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

/// An \ref Operation class for post operation
/// Adds a node to the XML when \ref run is called
class PostOperation : public SerializationOperation {
public:
	/// Constructor
	///
	/// \param[in] branch the name of the branch to modify
	/// \param[in] id the id attribute of the node to be added
	/// \param[in] kvp the KeyValuePairs of the node to be added
	PostOperation(std::wstring branch, std::wstring id, const KeyValuePairs& kvp);
	bool run(pugi::xml_document& document) override;
	virtual ~PostOperation() = default;

private:
	/// The new task contents.
	KeyValuePairs task;
};
}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_POST_OPERATION_H_

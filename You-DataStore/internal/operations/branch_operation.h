//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_BRANCH_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_BRANCH_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {

/// BranchOperation gets the XML node in the document with the name specified
/// or create one if none exists
class BranchOperation : public Operation {
public:
	/// Get the handler of the XML node with the specified name
	/// in the specified XML document
	///
	/// \param[out] document the XML document to get the handler from
	/// \param[in] nodeName the name of the node to get the handler of
	static pugi::xml_node get(pugi::xml_document& document,
		std::wstring nodeName);
};
}  // namespace Internal
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_BRANCH_OPERATION_H_

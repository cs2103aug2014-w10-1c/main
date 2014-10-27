#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATIONS_ERASE_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATIONS_ERASE_OPERATION_H_

#include "../operation.h"

namespace You {
namespace DataStore {
namespace Internal {

/// An \ref Operation class for erase operation
/// Erases a node from the XML when \ref run is called
class EraseOperation : public Operation {
public:
	/// Constructor
	///
	/// \param[in] branch the name of the branch to modify
	/// \param[in] id the id attribute of the node to be erased
	EraseOperation(std::wstring branch, std::wstring id);
	bool run(pugi::xml_node& document) override;
	virtual ~EraseOperation() = default;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You
#endif  // YOU_DATASTORE_INTERNAL_OPERATIONS_ERASE_OPERATION_H_

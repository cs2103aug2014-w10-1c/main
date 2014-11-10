//@author A0114171W
#pragma once
#ifndef YOU_DATASTORE_INTERNAL_OPERATION_H_
#define YOU_DATASTORE_INTERNAL_OPERATION_H_

#include "../task_typedefs.h"
#include "pugixml.h"

namespace You {
namespace DataStore {
namespace Internal {

/// A pure virtual class of operations to be put into transaction stack
class Operation {
public:
	/// Executes the operation
	///
	/// \param[out] node The XML node to be modified
	/// \return True if the operation succeeded.
	virtual bool run(pugi::xml_node& node) = 0;

	virtual ~Operation() = default;

protected:
	/// The "id" attribute of the XML node to modify
	std::wstring nodeId;

	/// The name of the XML branch to modify
	std::wstring branchName;
};

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

#endif  // YOU_DATASTORE_INTERNAL_OPERATION_H_

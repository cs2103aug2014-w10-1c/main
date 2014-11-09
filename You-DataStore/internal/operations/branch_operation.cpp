//@author A0114171W
#include "stdafx.h"
#include "branch_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

pugi::xml_node BranchOperation::get(pugi::xml_node& node,
	std::wstring nodeName) {
	pugi::xml_node newNode = node.child(nodeName.c_str());
	if (newNode.empty()) {
		newNode = node.append_child(nodeName.c_str());
	}
	return newNode;
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

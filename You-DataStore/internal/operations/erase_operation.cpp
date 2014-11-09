//@author A0114171W
#include "stdafx.h"
#include "branch_operation.h"
#include "erase_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

EraseOperation::EraseOperation(std::wstring branch, std::wstring id) {
	branchName = branch;
	nodeId = id;
}

bool EraseOperation::run(pugi::xml_node& node) {
	pugi::xml_node xmlBranch = BranchOperation::get(node, branchName);
	pugi::xml_node toErase =
		xmlBranch.find_child_by_attribute(L"id", nodeId.c_str());
	return xmlBranch.remove_child(toErase);
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

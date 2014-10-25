#include "stdafx.h"
#include "branch_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

pugi::xml_node BranchOperation::get(pugi::xml_document& document,
	std::wstring nodeName) {
	pugi::xml_node node = document.child(nodeName.c_str());
	if (node.empty()) {
		node = document.append_child(nodeName.c_str());
	}
	return node;
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

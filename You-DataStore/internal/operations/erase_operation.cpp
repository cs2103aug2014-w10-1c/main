#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "erase_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

EraseOperation::EraseOperation(std::wstring branch, std::wstring id) {
	branchName = branch;
	nodeId = id;
}

bool EraseOperation::run(pugi::xml_node& document) {
	pugi::xml_node toErase =
		document.find_child_by_attribute(L"id", nodeId.c_str());
	return document.remove_child(toErase);
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

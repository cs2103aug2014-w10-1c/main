#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "../../task_typedefs.h"
#include "../internal_datastore.h"
#include "post_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PostOperation::PostOperation(std::wstring branch, std::wstring id, const KeyValuePairs& kvp) {
	branchName = branch;
	nodeId = id;
	task = kvp;
}

bool PostOperation::run(pugi::xml_document& document) {
	if (document.find_child_by_attribute(L"id", nodeId.c_str())) {
		return false;
	}
	pugi::xml_node newTask = document.append_child(L"task");

	pugi::xml_attribute id = newTask.append_attribute(L"id");
	id.set_value(nodeId.c_str());
	serialize(task, newTask);
	return true;
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

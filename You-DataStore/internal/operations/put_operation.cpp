#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "../../task_typedefs.h"
#include "../internal_datastore.h"
#include "post_operation.h"
#include "put_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PutOperation::PutOperation(std::wstring branch, std::wstring id, const KeyValuePairs& kvp) {
	branchName = branch;
	nodeId = id;
	task = kvp;
}

bool PutOperation::run(pugi::xml_node& document) {
	pugi::xml_node toEdit =
		document.find_child_by_attribute(L"id", nodeId.c_str());
	if (!toEdit) {
		return false;
	}

	document.remove_child(toEdit);

	PostOperation post(nodeId, task);
	return post.run(document);
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

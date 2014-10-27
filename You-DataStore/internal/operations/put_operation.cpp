#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "../../task_typedefs.h"
#include "../internal_datastore.h"
#include "post_operation.h"
#include "put_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PutOperation::PutOperation(TaskId id, const KeyValuePairs& stask) {
	taskId = id;
	task = stask;
}

bool PutOperation::run(pugi::xml_node& document) {
	std::wstring idString = boost::lexical_cast<std::wstring>(taskId);
	pugi::xml_node toEdit =
		document.find_child_by_attribute(L"id", idString.c_str());
	if (!toEdit) {
		return false;
	}

	document.remove_child(toEdit);

	PostOperation post(taskId, task);
	return post.run(document);
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

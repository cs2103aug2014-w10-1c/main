#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "../../task_typedefs.h"
#include "../internal_datastore.h"
#include "post_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

PostOperation::PostOperation(TaskId id, const SerializedTask& stask) {
	taskId = id;
	task = stask;
}

bool PostOperation::run(pugi::xml_document& document) {
	// Consider changing parameter to std::wstring altogether
	std::wstring idString = boost::lexical_cast<std::wstring>(taskId);
	if (document.find_child_by_attribute(L"id", idString.c_str())) {
		return false;
	}
	pugi::xml_node newTask = document.append_child(L"task");

	pugi::xml_attribute id = newTask.append_attribute(L"id");
	id.set_value(idString.c_str());
	serialize(task, newTask);
	return true;
}

}  // namespace Internal
}   // namespace DataStore
}   // namespace You

#include "stdafx.h"
#include <boost/lexical_cast.hpp>
#include "erase_operation.h"

namespace You {
namespace DataStore {
namespace Internal {

EraseOperation::EraseOperation(TaskId id) {
	taskId = id;
}

bool EraseOperation::run(pugi::xml_document& document) {
	std::wstring taskId = boost::lexical_cast<std::wstring>(taskId);
	pugi::xml_node toErase =
		document.find_child_by_attribute(L"id", taskId.c_str());
	return document.remove_child(toErase);
}

}  // namespace Internal
}  // namespace DataStore
}  // namespace You

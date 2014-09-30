/// \author A0112054Y
#include "stdafx.h"

#include "delete_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Response DeleteTask::execute() {
	return std::wstring(L"DELETE");
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

/// \author A0112054Y
#include "stdafx.h"

#include "filter_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Response FilterTask::execute() {
	return std::wstring(L"FILTER");
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

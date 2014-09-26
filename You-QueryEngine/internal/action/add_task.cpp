#include "stdafx.h"

#include "add_task.h"

namespace You {
namespace QueryEngine {
namespace Internal {

Response AddTask::execute() {
	return std::wstring(L"ADD");
}

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

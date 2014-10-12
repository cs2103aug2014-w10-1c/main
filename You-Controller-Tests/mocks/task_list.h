#pragma once
#ifndef YOU_CONTROLLER_TESTS_MOCKS_TASK_LIST_H_
#define YOU_CONTROLLER_TESTS_MOCKS_TASK_LIST_H_

#include "result.h"

namespace You {
namespace Controller {
namespace UnitTests {
namespace Mocks {

/// A dummy task list. This contains one task.
class TaskList : public You::Controller::TaskList {
public:
	/// Constructor. Creates the specified number of tasks.
	TaskList(size_t count = 1);
	
private:
	/// Creates a new task and adds it to the task list.
	Task createTask();
};

}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_TESTS_MOCKS_TASK_LIST_H_

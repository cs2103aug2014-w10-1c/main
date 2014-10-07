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
	TaskList();
};

}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_TESTS_MOCKS_TASK_LIST_H_

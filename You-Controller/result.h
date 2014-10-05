#pragma once
#ifndef YOU_CONTROLLER_RESULT_H_
#define YOU_CONTROLLER_RESULT_H_

#include "You-NLP/task_list.h"

namespace You {
namespace Controller {

/// Defines the result of a natural language query.
typedef QueryEngine::Response Result;

/// Defines the type of a task.
typedef QueryEngine::Task Task;

/// Defines the type of a task list.
typedef NLP::TaskList TaskList;

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_RESULT_H_

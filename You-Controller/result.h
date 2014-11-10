//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_RESULT_H_
#define YOU_CONTROLLER_RESULT_H_

#include <vector>
#include "You-QueryEngine/api.h"

namespace You {
namespace Controller {

/// Defines the type of a task.
typedef QueryEngine::Task Task;

/// Defines the type of a task list.
typedef std::vector<Task> TaskList;

/// Defines the query engine utility class.
typedef You::QueryEngine::QueryEngine QueryEngine;

}  // namespace Controller
}  // namespace You

#include "results/add_result.h"
#include "results/show_result.h"
#include "results/delete_result.h"
#include "results/edit_result.h"
#include "results/undo_result.h"
#include "results/result.h"

#endif  // YOU_CONTROLLER_RESULT_H_

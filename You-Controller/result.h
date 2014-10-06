//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_RESULT_H_
#define YOU_CONTROLLER_RESULT_H_

#include "You-QueryEngine/api.h"
#include "results/add_result.h"
#include "results/edit_result.h"
#include "results/delete_result.h"

namespace You {
namespace Controller {

/// Defines the type of a task.
typedef QueryEngine::Task Task;

/// Defines the type of a task list.
typedef std::vector<Task> TaskList;

/// Defines the result of a natural language query.
typedef boost::variant<
	ADD_RESULT,
	EDIT_RESULT,
	DELETE_RESULT
> Result;

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_RESULT_H_

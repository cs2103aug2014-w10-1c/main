//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_RESULTS_EDIT_RESULT_H_
#define YOU_CONTROLLER_RESULTS_EDIT_RESULT_H_

namespace You {
namespace Controller {

/// Defines the result of an Edit query.
struct EDIT_RESULT {
	/// The new, modified task. The task ID is guaranteed to be the same.
	Task::ID task;
};

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_RESULTS_EDIT_RESULT_H_

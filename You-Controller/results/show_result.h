//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_RESULTS_SHOW_RESULT_H_
#define YOU_CONTROLLER_RESULTS_SHOW_RESULT_H_

namespace You {
namespace Controller {

/// Defines the result of a Show query.
struct SHOW_RESULT {
	/// The tasks which were retrieved
	TaskList tasks;
};

}  // namespace Controller
}  // namespace You

#endif  // #ifndef YOU_CONTROLLER_RESULTS_SHOW_RESULT_H_


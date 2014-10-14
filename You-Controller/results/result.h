//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_RESULTS_RESULT_H_
#define YOU_CONTROLLER_RESULTS_RESULT_H_

#include <boost/variant.hpp>

namespace You {
namespace Controller {

/// Defines the type of the result from running a string query.
typedef boost::variant<
	ADD_RESULT,
	SHOW_RESULT,
	EDIT_RESULT,
	DELETE_RESULT
> Result;

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_RESULTS_RESULT_H_

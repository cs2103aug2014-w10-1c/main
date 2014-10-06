//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_RESULTS_RESULT_H_
#define YOU_CONTROLLER_RESULTS_RESULT_H_

#include <boost/variant.hpp>

namespace You {
namespace Controller {

typedef boost::variant<
	ADD_RESULT,
	EDIT_RESULT,
	DELETE_RESULT
> Result;

}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_RESULTS_RESULT_H_

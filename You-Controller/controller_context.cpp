//@author A0097630B
#include "stdafx.h"
#include "controller_context.h"

namespace You {
namespace Controller {

const Controller::Context Controller::Context::DEFAULT =
	Controller::Context();

Controller::Context::Context(const TaskList& taskList)
: context(taskList) {
}

Controller::Context::Context()
: context(const_cast<void*>(static_cast<void const*>(&DEFAULT))) {
}

bool Controller::Context::isDefault() const {
	void* const* result = boost::get<void*>(&context);
	return result != nullptr && *result == &DEFAULT;
}

}  // namespace Controller
}  // namespace You

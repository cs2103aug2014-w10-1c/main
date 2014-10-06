//@author A0097630B
#include "stdafx.h"
#include "controller_context.h"
#include "exceptions/context_required_exception.h"

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

const Task& Controller::Context::at(size_t offset) const {
	if (isDefault()) {
		throw ContextRequiredException();
	}

	return boost::get<TaskList>(context).at(offset);
}

const Task& Controller::Context::operator[](size_t offset) const {
	if (isDefault()) {
		throw ContextRequiredException();
	}

	return boost::get<TaskList>(context)[offset];
}

}  // namespace Controller
}  // namespace You

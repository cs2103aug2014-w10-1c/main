/// \file You-QueryEngine/internal/controller.h
/// Import various controller classes from controller.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_CONTROLLER_H_
#define YOU_QUERYENGINE_INTERNAL_CONTROLLER_H_

#include "controller/task_builder.h"
#include "controller/task_serializer.h"
#include "controller/task_graph_controller.h"

namespace You {
namespace QueryEngine {
namespace Internal {

class Controller {
public:
	/// Access the TaskGraph controller.
	typedef TaskGraphController Graph;
	/// Access the TaskBuilder.
	typedef TaskBuilder Builder;
	/// Access the TaskSerializer.
	typedef TaskSerializer Serializer;
private:
	Controller() = delete;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You
#endif  // YOU_QUERYENGINE_INTERNAL_CONTROLLER_H_

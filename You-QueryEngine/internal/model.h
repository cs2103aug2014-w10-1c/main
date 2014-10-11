/// \file You-QueryEngine/internal/model.h
/// Import various model classes.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_MODEL_H_
#define YOU_QUERYENGINE_INTERNAL_MODEL_H_

#include "../task_model.h"
#include "task_graph.h"
#include "state.h"

namespace You {
namespace QueryEngine {
namespace Internal {

class Model {
public:
	typedef Task Task;
	typedef TaskGraph TaskGraph;
	typedef State State;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_MODEL_H_

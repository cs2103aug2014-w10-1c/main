/// \file You-QueryEngine/internal/model.h
/// Import various model classes.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_INTERNAL_MODEL_H_
#define YOU_QUERYENGINE_INTERNAL_MODEL_H_

#include "../task_model.h"
#include "model/task_graph.h"
#include "model/state.h"

namespace You {
namespace QueryEngine {
namespace Internal {

class Model {
public:
	/// The task model.
	typedef Task Task;
	/// The TaskGraph model.
	typedef TaskGraph TaskGraph;
	/// The State of the query engine.
	typedef State State;
private:
	Model() = delete;
	Model(const Model&) = delete;
	Model& operator=(const Model&) = delete;
};

}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_INTERNAL_MODEL_H_

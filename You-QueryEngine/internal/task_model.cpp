#include "task_model.h"

namespace You {
namespace QueryEngine {

void Task::setDescription(const Description& description) {
	this->description = description;
}

void Task::setDeadline(Time time) {
	this->deadline = time;
}

void Task::setDependencies(const Dependencies& dependencies) {
	this->dependencies = dependencies;
}

void Task::setPriority(Priority priority) {
	this->priority = priority;
}

}  // namespace QueryEngine
}  // namespace You

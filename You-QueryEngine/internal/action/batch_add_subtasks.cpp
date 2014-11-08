//@author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"

#include "add_task.h"
#include "delete_task.h"
#include "batch_add_subtasks.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

using Log = You::Utils::Log;

const std::wstring BatchAddSubTasks::logCategory =
	Query::logCategory + L"[BatchAddSubtasks]";

std::unique_ptr<Query> BatchAddSubTasks::getReverse() {
	return std::unique_ptr<Query>(new DeleteTask(insertedID));
}

Task::Subtasks BatchAddSubTasks::executeSubtasksAddQuery(State& state) const {
	Task::Subtasks theSubtasks;
	for (auto& q : subtasks) {
		Response r = q->execute(state);
		theSubtasks.insert(boost::get<Task>(r).getID());
	}
	return theSubtasks;
}

Task BatchAddSubTasks::executeParentAddQuery(State& state,
	const Task::Subtasks& subtasks) const {
	std::unique_ptr<Query> addParentQuery;
	if (insertedID == -1) {
		addParentQuery = std::unique_ptr<AddTask>(new AddTask(description,
			startTime, deadline, priority, dependencies, subtasks));
	} else {
		addParentQuery = std::unique_ptr<AddTask>(new AddTask(insertedID, description,
			startTime, deadline, priority, dependencies, subtasks));
	}
	return boost::get<Task>(addParentQuery->execute(state));
}

Response BatchAddSubTasks::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : BEGIN") % logCategory).str();
	Task::Subtasks theSubtasks = executeSubtasksAddQuery(state);
	Task newTask = executeParentAddQuery(state, theSubtasks);
	insertedID = newTask.getID();
	Log::debug << (boost::wformat(L"%1% : END") % logCategory).str();
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

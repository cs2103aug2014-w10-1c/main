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

Response BatchAddSubTasks::execute(State& state) {
	Log::debug << (boost::wformat(L"%1% : BEGIN") % logCategory).str();
	Task newTask;
	Task::Subtasks theSubtasks;

	for (auto& q : subtasks) {
		Response r = q->execute(state);
		theSubtasks.insert(boost::get<Task>(r).getID());
	}

	auto addParentquery = (insertedID == -1)
		? std::unique_ptr<AddTask>(new AddTask(description,
			startTime, deadline, priority, dependencies, theSubtasks))
		: std::unique_ptr<AddTask>(new AddTask(insertedID, description,
			startTime, deadline, priority, dependencies, theSubtasks));

	Response r = addParentquery->execute(state);
	newTask = boost::get<Task>(r);
	insertedID = newTask.getID();

	Log::debug << (boost::wformat(L"%1% : END") % logCategory).str();
	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

/// \author A0112054Y
#include "stdafx.h"

#include "../../../You-Utils/log.h"

#include "add_task.h"
#include "batch_delete_subtasks.h"
#include "batch_add_subtasks.h"

namespace You {
namespace QueryEngine {
namespace Internal {
namespace Action {

using Log = You::Utils::Log;

const std::wstring BatchAddSubTasks::logCategory =
	Query::logCategory + L"[BatchAddSubTasks]";

std::unique_ptr<Query> BatchAddSubTasks::getReverse() {
	return std::unique_ptr<Query>(new BatchDeleteSubTasks(
		insertedID));
}

Response BatchAddSubTasks::execute(State& state) {
	Task newTask;
	Task::Subtasks theSubtasks;

	for (auto& q : subtasks) {
		Response r = q->execute(state);
		theSubtasks.insert(boost::get<Task>(r).getID());
	}

	auto addParentquery = (insertedID == -1)
		? std::unique_ptr<AddTask>(new AddTask(description,
			deadline, priority, dependencies, theSubtasks))
		: std::unique_ptr<AddTask>(new AddTask(insertedID, description,
			deadline, priority, dependencies, theSubtasks));

	Response r = addParentquery->execute(state);
	newTask = boost::get<Task>(r);
	insertedID = newTask.getID();

	return newTask;
}

}  // namespace Action
}  // namespace Internal
}  // namespace QueryEngine
}  // namespace You

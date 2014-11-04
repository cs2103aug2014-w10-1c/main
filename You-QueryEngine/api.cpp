/// \author A0112054Y
#include "stdafx.h"

#include "internal/controller.h"
#include "internal/action/add_task.h"
#include "internal/action/update_task.h"
#include "internal/action/delete_task.h"
#include "internal/action/get_task.h"
#include "internal/action/delete_task.h"
#include "internal/action/update_task.h"
#include "internal/action/undo.h"
#include "internal/action/batch_add_subtasks.h"
#include "internal/action/batch_delete_subtasks.h"
#include "internal/model.h"
#include "api.h"

namespace You {
namespace QueryEngine {

const std::wstring Query::logCategory = L"[QE]";

std::unique_ptr<Query>
Query::getReverse() {
	throw Exception::NotUndoAbleException();
}

std::unique_ptr<Query>
QueryEngine::BatchAddSubTasks(
	const Task::Description& description,
	const Task::Time& deadline,
	const Task::Priority& priority,
	const Task::Dependencies& dependencies,
	std::vector<std::unique_ptr<Query>>&& subtasks) {
	using BatchAddSubTasks = Internal::Action::BatchAddSubTasks;
	return std::unique_ptr<Query>(new BatchAddSubTasks(
		description,
		deadline,
		priority,
		dependencies,
		std::move(subtasks)));
}

std::unique_ptr<Query>
QueryEngine::BatchDeleteSubTasks(Task::ID id) {
	using BatchDeleteSubtasks = Internal::Action::BatchDeleteSubTasks;
	return std::unique_ptr<Query>(new BatchDeleteSubtasks(id));
}

std::unique_ptr<Query>
QueryEngine::AddTask(
	const Task::Description& description,
	const Task::Time& deadline,
	const Task::Priority& priority,
	const Task::Dependencies& dependencies,
	const Task::Subtasks& subtasks) {
	using AddTask = Internal::Action::AddTask;
	return std::unique_ptr<Query>(new AddTask(description, deadline,
		priority, dependencies, subtasks));
}

std::unique_ptr<Query>
QueryEngine::GetTask(const Filter& filter) {
	using GetTask = Internal::Action::GetTask;
	return std::unique_ptr<Query>(new GetTask(filter));
}

std::unique_ptr<Query>
QueryEngine::GetTask(const Filter& filter,
	const Comparator& comparator) {
	using GetTask = Internal::Action::GetTask;
	return std::unique_ptr<Query>(new GetTask(filter, comparator));
}

std::unique_ptr<Query>
QueryEngine::DeleteTask(Task::ID id) {
	using DeleteTask = Internal::Action::DeleteTask;
	return std::unique_ptr<Query>(new DeleteTask(id));
}

std::unique_ptr<Query>
QueryEngine::UpdateTask(Task::ID id,
	You::Utils::Option<Task::Description> description,
	You::Utils::Option<Task::Time> deadline,
	You::Utils::Option<Task::Priority> priority,
	You::Utils::Option<Task::Dependencies> dependencies,
	You::Utils::Option<bool> completed,
	You::Utils::Option<Task::ID> parent,
	You::Utils::Option<Task::Subtasks> subtasks,
	You::Utils::Option<Task::Attachment> attachment) {
	using UpdateTask = Internal::Action::UpdateTask;
	return std::unique_ptr<Query>(new UpdateTask(id, description,
		deadline, priority, dependencies, completed, parent, subtasks,
		attachment));
}

std::unique_ptr<Query>
QueryEngine::Undo() {
	using Undo = Internal::Action::Undo;
	return std::unique_ptr<Query>(new Undo());
}

Response QueryEngine::executeQuery(std::unique_ptr<Query> query) {
	Response response;
	std::unique_ptr<Query> reverse;
	response = query->execute(Internal::State::get());
	try {
		reverse = query->getReverse();
		Internal::State::get().undoStack().emplace(std::move(reverse));
	} catch (const Exception::NotUndoAbleException&) {
	}
	return response;
}

std::wstring ToString(const Task& task) {
	using Serializer = Internal::TaskSerializer;
	auto serialized = Serializer::serialize(task);
	std::wstring TASK_FORMAT = L"[%1%][%2%][%3%][%4%][%5%][%6%][%7%][%8%]";
	return (boost::wformat(TASK_FORMAT)
		% serialized.at(Serializer::KEY_ID)
		% serialized.at(Serializer::KEY_DESCRIPTION)
		% serialized.at(Serializer::KEY_PRIORITY)
		% serialized.at(Serializer::KEY_DEADLINE)
		% serialized.at(Serializer::KEY_DEPENDENCIES)
		% serialized.at(Serializer::KEY_PARENT)
		% serialized.at(Serializer::KEY_SUBTASKS)
		% serialized.at(Serializer::KEY_ATTACHMENT)).str();
}

}  // namespace QueryEngine
}  // namespace You


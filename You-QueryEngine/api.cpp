//@author A0112054Y
#include "stdafx.h"

#include "internal/action/add_task.h"
#include "internal/action/update_task.h"
#include "internal/action/delete_task.h"
#include "internal/action/get_task.h"
#include "internal/action/delete_task.h"
#include "internal/action/update_task.h"
#include "internal/action/undo.h"
#include "internal/action/batch_add_subtasks.h"
#include "internal/action/batch_add_dependencies.h"
#include "internal/state.h"
#include "internal/controller.h"
#include "api.h"

namespace You {
namespace QueryEngine {

const std::wstring Query::logCategory = L"[QE]";

using You::Utils::Option;
using You::Utils::make_option;
using Internal::Action::AddTask;
using Internal::Action::BatchAddSubTasks;
using Internal::Action::BatchAddDependencies;
using Internal::Action::GetTask;
using Internal::Action::DeleteTask;
using Internal::Action::Undo;
using Internal::Controller;

std::unique_ptr<Query>
Query::getReverse() {
	throw Exception::NotUndoAbleException();
}

std::unique_ptr<Query>
QueryEngine::AddTask(
	const Task::Description& description,
	const Task::Time& startTime,
	const Task::Time& deadline,
	const Task::Priority& priority,
	std::vector<std::unique_ptr<Query>>&& dependencies,
	std::vector<std::unique_ptr<Query>>&& subtasks) {
	assert(!(dependencies.size() > 0 && subtasks.size() > 0));
	if (dependencies.size() > 0) {
		return std::unique_ptr<Query>(new BatchAddDependencies(
				description, startTime, deadline, priority,
				std::move(dependencies), {}));
	} else if (subtasks.size() > 0) {
		return std::unique_ptr<Query>(new BatchAddSubTasks(
				description, startTime, deadline, priority,
				{}, std::move(subtasks)));
	} else {
		return std::unique_ptr<Query>(
			new Internal::Action::AddTask(
				description, startTime, deadline,
				priority, {}, {}));
	}
}

std::unique_ptr<Query>
QueryEngine::GetTask() {
	return std::unique_ptr<Query>(
		new Internal::Action::GetTask(
			Internal::State::get().getActiveFilter(),
			Internal::State::get().getActiveComparator()));
}

std::unique_ptr<Query>
QueryEngine::GetTask(const Filter& filter) {
	return std::unique_ptr<Query>(
		new Internal::Action::GetTask(filter,
			Comparator::byTimeCreated()));
}

std::unique_ptr<Query>
QueryEngine::GetTask(const Filter& filter,
	const Comparator& comparator) {
	return std::unique_ptr<Query>(
		new Internal::Action::GetTask(filter, comparator));
}

std::unique_ptr<Query>
QueryEngine::DeleteTask(Task::ID id) {
	return std::unique_ptr<Query>(
		new Internal::Action::DeleteTask(id));
}

std::unique_ptr<Query>
QueryEngine::UpdateTask(Task::ID id,
	Option<Task::Description> description,
	Option<Task::Time> startTime,
	Option<Task::Time> deadline,
	Option<Task::Priority> priority,
	Delta<Task::Dependencies::value_type> dependencies,
	Option<bool> completed,
	Option<Task::ID> parent,
	Delta<Task::Subtasks::value_type> subtasks,
	Delta<Task::Attachment::value_type> attachment) {
	using UpdateTask = Internal::Action::UpdateTask;
	Task task = Internal::State::get().graph().getTask(id);
	auto newSubtasks =
		subtasksFromDelta(subtasks, task);
	auto newDependencies =
		dependenciesFromDelta(dependencies, task);
	auto newAttachment =
		attachmentsFromDelta(attachment, task);
	return std::unique_ptr<Query>(
		new UpdateTask(id, description,
			startTime, deadline, priority,
			newDependencies, completed,
			parent, newSubtasks, newAttachment));
}

std::unique_ptr<Query>
QueryEngine::UpdateTask(const Task& task) {
	using UpdateTask = Internal::Action::UpdateTask;
	return std::unique_ptr<Query>(new UpdateTask(task));
}

std::unique_ptr<Query>
QueryEngine::Undo() {
	return std::unique_ptr<Query>(new Internal::Action::Undo());
}

std::unique_ptr<Query>
QueryEngine::AddDependency(Task::ID id, Task::ID dependency) {
	Task task = Internal::State::get().graph().getTask(id);
	Task::Dependencies dependencies = task.getDependencies();
	dependencies.insert(dependency);
	task.setDependencies(dependencies);
	return std::unique_ptr<Query>(new Internal::Action::UpdateTask(task));
}

std::unique_ptr<Query>
QueryEngine::RemoveDependency(Task::ID id, Task::ID dependency) {
	Task task = Internal::State::get().graph().getTask(id);
	Task::Dependencies dependencies = task.getDependencies();
	dependencies.erase(dependency);
	task.setDependencies(dependencies);
	return std::unique_ptr<Query>(new Internal::Action::UpdateTask(task));
}

std::unique_ptr<Query>
QueryEngine::AddSubtask(Task::ID id, Task::ID subtask) {
	Task task = Internal::State::get().graph().getTask(id);
	Task::Subtasks subtasks = task.getSubtasks();
	subtasks.insert(subtask);
	task.setSubtasks(subtasks);
	return std::unique_ptr<Query>(new Internal::Action::UpdateTask(task));
}

std::unique_ptr<Query>
QueryEngine::RemoveSubtask(Task::ID id, Task::ID subtask) {
	Task task = Internal::State::get().graph().getTask(id);
	Task::Subtasks subtasks = task.getSubtasks();
	subtasks.erase(subtask);
	task.setSubtasks(subtasks);
	return std::unique_ptr<Query>(new Internal::Action::UpdateTask(task));
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

Option<Task::Attachment> QueryEngine::attachmentsFromDelta(
	const Delta<Task::Attachment::value_type>& attachment,
	const Task& task) {
	Task::Attachment att = task.getAttachment();
	if (attachment.type ==
			Delta<Task::Attachment::value_type>::Type::ADD) {
		att.insert(
			end(att),
			begin(attachment.elements),
			end(attachment.elements));
	} else {
		for (const auto& a : attachment.elements) {
			auto pos = std::find(begin(att), end(att), a);
			if (pos != end(att)) {
				att.erase(pos);
			}
		}
	}
	return make_option<Task::Attachment>(att);
}

Option<Task::Subtasks> QueryEngine::subtasksFromDelta(
	const Delta<Task::Subtasks::value_type>& subtasks,
	const Task& task) {
	Task::Subtasks subs= task.getSubtasks();
	if (subtasks.type ==
			Delta<Task::Subtasks::value_type>::Type::ADD) {
		for (const auto& s : subtasks.elements) {
			subs.insert(s);
		}
	} else {
		for (const auto& s : subtasks.elements) {
			subs.erase(s);
		}
	}
	return make_option<Task::Subtasks>(subs);
}

Option<Task::Dependencies> QueryEngine::dependenciesFromDelta(
	const Delta<Task::Dependencies::value_type>& dependencies,
	const Task& task) {
	Task::Dependencies deps = task.getDependencies();
	if (dependencies.type ==
			Delta<Task::Dependencies::value_type>::Type::ADD) {
		for (const auto& s : dependencies.elements) {
			deps.insert(s);
		}
	} else {
		for (const auto& s : dependencies.elements) {
			deps.erase(s);
		}
	}
	return make_option<Task::Dependencies>(deps);
}

std::wstring ToString(const Task& task) {
	using Serializer = Controller::Serializer;
	auto serialized = Serializer::serialize(task);
	std::wstring TASK_FORMAT = L"[%1%][%2%][%3%][%4%][%5%][%6%][%7%][%8%][%9%]";
	return (boost::wformat(TASK_FORMAT)
		% serialized.at(Serializer::KEY_ID)
		% serialized.at(Serializer::KEY_DESCRIPTION)
		% serialized.at(Serializer::KEY_START_TIME)
		% serialized.at(Serializer::KEY_PRIORITY)
		% serialized.at(Serializer::KEY_DEADLINE)
		% serialized.at(Serializer::KEY_DEPENDENCIES)
		% serialized.at(Serializer::KEY_PARENT)
		% serialized.at(Serializer::KEY_SUBTASKS)
		% serialized.at(Serializer::KEY_ATTACHMENT)).str();
}

}  // namespace QueryEngine
}  // namespace You


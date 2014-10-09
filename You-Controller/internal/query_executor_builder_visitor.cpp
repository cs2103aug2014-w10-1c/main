//@author A0097630B
#include "stdafx.h"
#include "query_executor.h"
#include "../exceptions/context_index_out_of_range_exception.h"
#include "../result.h"
#include "query_executor_builder_visitor.h"

namespace You {
namespace Controller {
namespace Internal {

using You::NLP::QUERY;
using You::NLP::ADD_QUERY;
using You::NLP::EDIT_QUERY;
using You::NLP::DELETE_QUERY;

QueryExecutorBuilderVisitor::QueryExecutorBuilderVisitor(
	const Controller::Context& context)
	: context(context) {
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const ADD_QUERY& query) {
	class AddTaskQueryExecutor : public QueryExecutor {
	public:
		explicit AddTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~AddTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return ADD_RESULT {
				boost::get<Task>(response)
			};
		}
	};

	return std::unique_ptr<QueryExecutor>(
		new AddTaskQueryExecutor(
			QueryEngine::AddTask(
				query.description,
				query.deadline ? query.deadline.get() : Task::DEFAULT_DEADLINE,
				query.priority == You::NLP::TaskPriority::HIGH ?
					Task::Priority::HIGH : Task::Priority::NORMAL,
				Task::Dependencies()
			)
		)
	);
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const EDIT_QUERY& query) const {
	class EditTaskQueryExecutor : public QueryExecutor {
	public:
		explicit EditTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~EditTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return EDIT_RESULT {
				boost::get<Task>(response)
			};
		}
	};

	try {
		const Task& task = context.at(query.taskID);
		std::wstring description = query.description ?
			query.description.get() :
			Task::DEFAULT_DESCRIPTION;
		boost::posix_time::ptime deadline = query.deadline ?
			query.deadline.get() :
			Task::DEFAULT_DEADLINE;
		Task::Priority priority = query.priority ?
			(query.priority == You::NLP::TaskPriority::HIGH ?
				Task::Priority::HIGH : Task::Priority::NORMAL) :
			Task::DEFAULT_PRIORITY;

		return std::unique_ptr<QueryExecutor>(
			new EditTaskQueryExecutor(
				QueryEngine::UpdateTask(
					task.getID(),
					description,
					deadline,
					Task::DEFAULT_PRIORITY,
					Task::Dependencies())));
	} catch (std::out_of_range& e) {
		throw ContextIndexOutOfRangeException(e);
	}
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const DELETE_QUERY& query) const {
	class DeleteTaskQueryExecutor : public QueryExecutor {
	public:
		explicit DeleteTaskQueryExecutor(
			std::unique_ptr<QueryEngine::Query>&& query)
			: QueryExecutor(std::move(query)) {
		}

		virtual ~DeleteTaskQueryExecutor() = default;

	protected:
		Result processResponse(
			const You::QueryEngine::Response& response) override {
			return DELETE_RESULT {
				boost::get<Task::ID>(response)
			};
		}
	};

	try {
		const Task& task = context.at(query.taskID);

		return std::unique_ptr<QueryExecutor>(
			new DeleteTaskQueryExecutor(
				QueryEngine::DeleteTask(
					task.getID())));
	} catch (std::out_of_range& e) {
		throw ContextIndexOutOfRangeException(e);
	}
}

}  // namespace Internal
}  // namespace Controller
}  // namespace You

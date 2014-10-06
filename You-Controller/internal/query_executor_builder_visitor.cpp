//@author A0097630B
#include "stdafx.h"
#include "query_executor.h"
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
				query.due ? query.due.get() : Task::DEFAULT_DEADLINE,
				Task::Priority::NORMAL,
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

	const Task& task = context.at(query.taskID);
	std::wstring description =
		(query.fieldsToChange & EDIT_QUERY::FIELDS::DESCRIPTION) ==
			EDIT_QUERY::FIELDS::NONE ?
				Task::DEFAULT_DESCRIPTION :
				query.description;
	boost::posix_time::ptime due =
		(query.fieldsToChange & EDIT_QUERY::FIELDS::DUE) ==
			EDIT_QUERY::FIELDS::NONE ?
				Task::DEFAULT_DEADLINE :
				query.due;


	return std::unique_ptr<QueryExecutor>(
		new EditTaskQueryExecutor(
			QueryEngine::UpdateTask(
				task.getID(),
				description,
				due,
				Task::DEFAULT_PRIORITY,
				Task::Dependencies()
			)
		)
	);
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const DELETE_QUERY& query) const {
	// TODO(lowjoel): waiting for API
	return nullptr;
}

}  // namespace Internal
}  // namespace Controller
}  // namespace You

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
QueryExecutorBuilderVisitor::build(const EDIT_QUERY& query) {
	// TODO(lowjoel): waiting for API
	return nullptr;
}

std::unique_ptr<QueryExecutor>
QueryExecutorBuilderVisitor::build(const DELETE_QUERY& query) {
	// TODO(lowjoel): waiting for API
	return nullptr;
}

}  // namespace Internal
}  // namespace Controller
}  // namespace You

//@author A0097630B
#include "stdafx.h"
#include "query_executor.h"
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
	QueryEngine::AddTask(
		query.description,
		query.due ? query.due.get() : Task::DEFAULT_DEADLINE,
		Task::Priority::NORMAL,
		Task::Dependencies());
	return nullptr;
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

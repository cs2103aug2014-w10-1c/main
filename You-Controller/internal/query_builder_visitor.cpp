//@author A0097630B
#include "stdafx.h"
#include "query_builder_visitor.h"

namespace You {
namespace Controller {
namespace Internal {

using You::QueryEngine::Query;
using You::NLP::QUERY;
using You::NLP::ADD_QUERY;
using You::NLP::EDIT_QUERY;
using You::NLP::DELETE_QUERY;

QueryBuilderVisitor::QueryBuilderVisitor(
	const Controller::Context& context)
	: context(context) {
}

std::unique_ptr<Query>
QueryBuilderVisitor::build(const ADD_QUERY& query) {
	return QueryEngine::AddTask(
		query.description,
		query.due ? query.due.get() : Task::DEFAULT_DEADLINE,
		Task::Priority::NORMAL,
		Task::Dependencies());
}

std::unique_ptr<Query>
QueryBuilderVisitor::build(const EDIT_QUERY& query) {
	// TODO(lowjoel): waiting for API
	return nullptr;
}

std::unique_ptr<Query>
QueryBuilderVisitor::build(const DELETE_QUERY& query) {
	// TODO(lowjoel): waiting for API
	return nullptr;
}

}  // namespace Internal
}  // namespace Controller
}  // namespace You

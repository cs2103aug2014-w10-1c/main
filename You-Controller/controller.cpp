#include "stdafx.h"
#include "controller.h"
#include "You-NLP/query_parser.h"
#include "You-NLP/parse_tree.h"
#include "internal/query_builder_visitor.h"

namespace You {
namespace Controller {

using You::NLP::QueryParser;
using Date = boost::gregorian::date;
using QueryParseTree = You::NLP::QUERY;
using AbstractQuery = QueryEngine::Query;
using QueryBuilderVisitor = Internal::QueryBuilderVisitor;

Controller Controller::instance;

Controller& Controller::get() {
	return instance;
}

Result Controller::query(
	const std::wstring& query,
	const Controller::Context& context) const {
	QueryParseTree parseTree = QueryParser::parse(query);

	QueryBuilderVisitor visitor(context);
	std::unique_ptr<AbstractQuery> queryRef =
		boost::apply_visitor(visitor, parseTree);

	QueryEngine::Response response = QueryEngine::executeQuery(
		std::move(queryRef));
	return response;
}

Result Controller::getTasks(const std::vector<Task::ID>& taskIDs) const {
	std::unique_ptr<AbstractQuery> query =
		QueryEngine::FilterTask(QueryEngine::Filter::idIsIn(taskIDs));

	return QueryEngine::executeQuery(std::move(query));
}

Result Controller::getTasks() const {
	std::unique_ptr<AbstractQuery> query =
		QueryEngine::FilterTask(QueryEngine::Filter::anyTask());

	return QueryEngine::executeQuery(std::move(query));
}

}  // namespace Controller
}  // namespace You

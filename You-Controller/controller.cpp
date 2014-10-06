//@author A0097630B
#include "stdafx.h"
#include "controller.h"
#include "You-NLP/query_parser.h"
#include "You-NLP/parse_tree.h"
#include "internal/query_executor.h"
#include "internal/query_executor_builder_visitor.h"

namespace You {
namespace Controller {

using You::NLP::QueryParser;
using QueryParseTree = You::NLP::QUERY;
using AbstractQuery = QueryEngine::Query;

Controller Controller::instance;

Controller& Controller::get() {
	return instance;
}

Result Controller::query(
	const std::wstring& query,
	const Controller::Context& context) const {
	QueryParseTree parseTree = QueryParser::parse(query);

	Internal::QueryExecutorBuilderVisitor visitor(context);
	std::unique_ptr<Internal::QueryExecutor> executor(
		boost::apply_visitor(visitor, parseTree));

	return executor->execute();
}

TaskList Controller::getTasks(const std::vector<Task::ID>& taskIDs) const {
	std::unique_ptr<AbstractQuery> query =
		QueryEngine::FilterTask(QueryEngine::Filter::idIsIn(taskIDs));

	return boost::get<TaskList>(QueryEngine::executeQuery(std::move(query)));
}

TaskList Controller::getTasks() const {
	std::unique_ptr<AbstractQuery> query =
		QueryEngine::FilterTask(QueryEngine::Filter::anyTask());

	return boost::get<TaskList>(QueryEngine::executeQuery(std::move(query)));
}

}  // namespace Controller
}  // namespace You

//@author A0097630B
#include "stdafx.h"
#include "You-NLP/query_parser.h"
#include "You-NLP/parse_tree.h"
#include "internal/query_executor.h"
#include "internal/query_executor_builder_visitor.h"
#include "result.h"
#include "controller_context.h"
#include "controller.h"

namespace You {
namespace Controller {

using You::NLP::QueryParser;
using QueryParseTree = You::NLP::QUERY;
using AbstractQuery = QueryEngine::Query;

Controller Controller::instance;
const std::unordered_map<NLP::TaskPriority, Task::Priority>
Controller::nlpToQueryEnginePriorityMap({
	{ NLP::TaskPriority::NORMAL, Task::Priority::NORMAL },
	{ NLP::TaskPriority::HIGH, Task::Priority::HIGH }
});
const std::unordered_map<Task::Priority, NLP::TaskPriority>
Controller::queryEngineToNlpPriorityMap({
	{ Task::Priority::NORMAL, NLP::TaskPriority::NORMAL },
	{ Task::Priority::HIGH, NLP::TaskPriority::HIGH }
});

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
		QueryEngine::GetTask(QueryEngine::Filter::idIsIn(taskIDs));

	return boost::get<TaskList>(QueryEngine::executeQuery(std::move(query)));
}

TaskList Controller::getTasks() const {
	std::unique_ptr<AbstractQuery> query =
		QueryEngine::GetTask(QueryEngine::Filter::anyTask());

	return boost::get<TaskList>(QueryEngine::executeQuery(std::move(query)));
}

Task::Priority Controller::nlpToQueryEnginePriority(
	NLP::TaskPriority priority) {
	 auto iterator = nlpToQueryEnginePriorityMap.find(priority);
	 assert(iterator != end(nlpToQueryEnginePriorityMap));
	 return iterator->second;
}

NLP::TaskPriority Controller::queryEngineToNlpPriority(
	QueryEngine::Task::Priority priority) {
	auto iterator = queryEngineToNlpPriorityMap.find(priority);
	assert(iterator != end(queryEngineToNlpPriorityMap));
	return iterator->second;
}

}  // namespace Controller
}  // namespace You

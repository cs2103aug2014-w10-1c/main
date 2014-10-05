#include "stdafx.h"
#include "controller.h"
#include "internal/query_parser.h"
#include "internal/parse_tree.h"
#include "internal/date_time_parser.h"

namespace You {
namespace NLP {

using Internal::QueryParser;
using Internal::DateTimeParser;
using Internal::QUERY;
using Internal::ADD_QUERY;
using Internal::EDIT_QUERY;
using Internal::DELETE_QUERY;
using Date = boost::gregorian::date;
using Query = QueryEngine::Query;

Controller Controller::instance;

/// The query builder that will send the query to the appropriate builder
/// in the controller class.
class Controller::QueryBuilderVisitor : public boost::static_visitor<
	std::unique_ptr<Query>> {
public:
	/// Constructor. Specify the context for which the query is to be built
	/// with.
	explicit QueryBuilderVisitor(const Controller::Context& context);

	/// Visitor implementation.
	///
	/// \param[in] query The actual query from the parse tree.
	template<typename QueryType>
	std::unique_ptr<Query> operator()(
		const QueryType& query) const {
		return build(query);
	}

private:
	QueryBuilderVisitor(const QueryBuilderVisitor&) = delete;
	QueryBuilderVisitor& operator=(const QueryBuilderVisitor&) = delete;

	/// Builds a query engine query from the given add syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<Query> build(const ADD_QUERY& query);

	/// Builds a query engine query from the given edit syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<Query> build(const EDIT_QUERY& query);

	/// Builds a query engine query from the given delete syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<Query> build(const DELETE_QUERY& query);

private:
	/// The context for the query.
	const Controller::Context& context;
};

Controller& Controller::get() {
	return instance;
}

Result Controller::query(
	const std::wstring& query,
	const Controller::Context& context) const {
	QUERY parseTree = QueryParser::parse(query);

	QueryBuilderVisitor visitor(context);
	std::unique_ptr<Query> queryRef =
		boost::apply_visitor(visitor, parseTree);

	QueryEngine::Response response = QueryEngine::executeQuery(
		std::move(queryRef));
	return response;
}

Result Controller::getTasks(const std::vector<Task::ID>& taskIDs) const {
	std::unique_ptr<Query> query =
		QueryEngine::FilterTask([&taskIDs](const Task& task) {
			return std::find(taskIDs.begin(), taskIDs.end(), task.getID()) !=
				taskIDs.end();
		});

	return QueryEngine::executeQuery(std::move(query));
}

Result Controller::getTasks() const {
	std::unique_ptr<Query> query =
		QueryEngine::FilterTask([](const Task& /*task*/) {
			return true;
		});

	return QueryEngine::executeQuery(std::move(query));
}

Controller::QueryBuilderVisitor::QueryBuilderVisitor(
	const Controller::Context& context)
: context(context) {
}

std::unique_ptr<Query>
Controller::QueryBuilderVisitor::build(const ADD_QUERY& query) {
	return QueryEngine::AddTask(
		query.description,
		query.due,
		Task::Priority::NORMAL,
		Task::Dependencies());
}

std::unique_ptr<Query>
Controller::QueryBuilderVisitor::build(const EDIT_QUERY& query) {
	// TODO(lowjoel): waiting for API
	return nullptr;
}

std::unique_ptr<Query>
Controller::QueryBuilderVisitor::build(const DELETE_QUERY& query) {
	// TODO(lowjoel): waiting for API
	return nullptr;
}

}  // namespace NLP
}  // namespace You

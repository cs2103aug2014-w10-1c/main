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
using Date = boost::gregorian::date;

/// The query builder that will send the query to the appropriate builder
/// in the controller class.
class Controller::QueryBuilderVisitor : public boost::static_visitor<
	std::shared_ptr<QueryEngine::Query>> {
public:
	/// Constructor. Specify the context for which the query is to be built
	/// with.
	explicit QueryBuilderVisitor(const Controller::Context& context);

	/// Visitor implementation.
	///
	/// \param[in] query The actual query from the parse tree.
	template<typename QueryType>
	std::shared_ptr<QueryEngine::Query> operator()(
		const QueryType& query) const {
		return build(query);
	}

private:
	QueryBuilderVisitor(const QueryBuilderVisitor&) = delete;
	QueryBuilderVisitor& operator=(const QueryBuilderVisitor&) = delete;

	/// Builds a query engine query from the given syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::shared_ptr<QueryEngine::Query> build(const ADD_QUERY& query);

	/// Converts a boost::gregorian::date to a time_t.
	///
	/// \param[in] date The date to convert.
	static time_t dateToTimeT(const Date& date);

private:
	/// The context for the query.
	const Controller::Context& context;
};

Result Controller::query(
	const std::wstring& query,
	const Controller::Context& context) const {
	QUERY parseTree = QueryParser::parse(query);

	QueryBuilderVisitor visitor(context);
	std::shared_ptr<QueryEngine::Query> queryRef =
		boost::apply_visitor(visitor, parseTree);

	QueryEngine::Response response = QueryEngine::executeQuery(queryRef);
	return response;
}

Controller::QueryBuilderVisitor::QueryBuilderVisitor(
	const Controller::Context& context)
: context(context) {
}

std::shared_ptr<QueryEngine::Query>
Controller::QueryBuilderVisitor::build(const ADD_QUERY& query) {
	const std::wstring& description = query.description;
	boost::gregorian::date deadline = DateTimeParser::parse(query.due);

	return QueryEngine::AddTask(
		query.description,
		dateToTimeT(deadline),
		Task::Priority::NORMAL,
		Task::Dependencies());
}

}  // namespace NLP
}  // namespace You

//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_INTERNAL_QUERY_BUILDER_VISITOR_H_
#define YOU_CONTROLLER_INTERNAL_QUERY_BUILDER_VISITOR_H_

#include <memory>
#include <boost/variant.hpp>

#include "../controller_context.h"
#include "You-NLP/parse_tree.h"

namespace You {
namespace QueryEngine { class Query; }
namespace Controller {
namespace Internal {

/// The query builder that will send the query to the appropriate builder
/// in the controller class.
///
/// This translates a given syntax tree returned by the \ref QueryParser and
/// then constructs the corresponding query object that can be used by the
/// Query Engine.
class QueryBuilderVisitor : public boost::static_visitor<
	std::unique_ptr<You::QueryEngine::Query>> {
public:
	/// Constructor. Specify the context for which the query is to be built
	/// with.
	explicit QueryBuilderVisitor(const Controller::Context& context);

	/// Visitor implementation.
	///
	/// \param[in] query The actual query from the parse tree.
	template<typename QueryType>
	std::unique_ptr<You::QueryEngine::Query> operator()(
		const QueryType& query) const {
		return build(query);
	}

private:
	QueryBuilderVisitor(const QueryBuilderVisitor&) = delete;
	QueryBuilderVisitor& operator=(const QueryBuilderVisitor&) = delete;

	/// Builds a query engine query from the given add syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<You::QueryEngine::Query>
		build(const You::NLP::ADD_QUERY& query);

	/// Builds a query engine query from the given edit syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<You::QueryEngine::Query>
		build(const You::NLP::EDIT_QUERY& query);

	/// Builds a query engine query from the given delete syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<You::QueryEngine::Query>
		build(const You::NLP::DELETE_QUERY& query);

private:
	/// The context for the query.
	const You::Controller::Controller::Context& context;
};


}  // namespace Internal
}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_INTERNAL_QUERY_BUILDER_VISITOR_H_

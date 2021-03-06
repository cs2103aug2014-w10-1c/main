//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_INTERNAL_QUERY_EXECUTOR_BUILDER_VISITOR_H_
#define YOU_CONTROLLER_INTERNAL_QUERY_EXECUTOR_BUILDER_VISITOR_H_

#include <memory>
#include <boost/variant.hpp>

#include "../controller_context.h"
#include "You-NLP/parse_tree.h"

namespace You {
namespace QueryEngine { class Query; }
namespace Controller {
namespace Internal {

/// The query executor builder that will convert a syntax tree from the
/// \ref You::NLP::QueryParser into a \ref QueryExecutor object.
class QueryExecutorBuilderVisitor : public boost::static_visitor<
	std::unique_ptr<class QueryExecutor>> {
public:
	/// Constructor. Specify the context for which the query is to be built
	/// with.
	explicit QueryExecutorBuilderVisitor(const Controller::Context& context);

	/// Visitor implementation.
	///
	/// \param[in] query The actual query from the parse tree.
	template<typename QueryType>
	std::unique_ptr<QueryExecutor> operator()(
		const QueryType& query) const {
		return build(query);
	}

private:
	QueryExecutorBuilderVisitor(const QueryExecutorBuilderVisitor&) = delete;
	QueryExecutorBuilderVisitor& operator=(
		const QueryExecutorBuilderVisitor&) = delete;

	/// Builds a query engine query from the given add syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<QueryExecutor>
		build(const You::NLP::ADD_QUERY& query);

	/// Builds a query engine query from the given add syntax tree. This will
	/// recursively convert subtasks.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<You::QueryEngine::Query> buildAddQuery(
		const You::NLP::ADD_QUERY& query);

	/// Builds a query engine query from the given show syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	static std::unique_ptr<QueryExecutor>
		build(const You::NLP::SHOW_QUERY& query);

	/// Builds a query engine filter functor from the given task getter,
	/// predicate, and value to compare against.
	///
	/// \tparam TValue The return type of the getter.
	/// \param selector The selector to get the property of the task.
	/// \param predicate The predicate to use when comparing against the value.
	/// \param value The value to compare the task property against.
	template<typename TValue>
	static std::function<bool(const Task&)> buildComparator(
		TValue (QueryEngine::Task::*selector)() const,
		You::NLP::SHOW_QUERY::Predicate predicate,
		const TValue& value);

	/// Builds a query engine query from the given edit syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	std::unique_ptr<QueryExecutor>
		build(const You::NLP::EDIT_QUERY& query) const;

	/// Builds a query engine query from the given delete syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	std::unique_ptr<QueryExecutor>
		build(const You::NLP::DELETE_QUERY& query) const;

	/// Builds a query engine query from the given undo syntax tree.
	///
	/// \param[in] query The syntax tree to build a query from.
	std::unique_ptr<QueryExecutor>
		build(const You::NLP::UNDO_QUERY& query) const;

private:
	/// The context for the query.
	You::Controller::Controller::Context context;
};


}  // namespace Internal
}  // namespace Controller
}  // namespace You

#endif  // YOU_CONTROLLER_INTERNAL_QUERY_EXECUTOR_BUILDER_VISITOR_H_

//@author A0097630B
#pragma once
#ifndef YOU_CONTROLLER_INTERNAL_QUERY_EXECUTOR_H_
#define YOU_CONTROLLER_INTERNAL_QUERY_EXECUTOR_H_

#include <memory>

#include "You-QueryEngine/api.h"
#include "../result.h"

namespace You {
namespace Controller {
namespace Internal {

/// Forms an abstract base class for all query executors.
///
/// Query Executors accept a query object, returned from the Query Builder.
/// The Query Executor has an associated method to parse the response from the
/// Query Builder, returning the appropriate result to the consumers of the
/// controller.
class QueryExecutor {
public:
	/// Default virtual constructor.
	virtual ~QueryExecutor() = default;

	/// Executes the query, returning the concrete result.
	///
	/// \return The concrete result of the query.
	Result execute() const;

protected:
	/// Constructor.
	///
	/// \param[in] query The query which was retrieved from the Query Engine.
	explicit QueryExecutor(std::unique_ptr<You::QueryEngine::Query>&& query);

	/// Processes the response from the query engine, returning a result.
	///
	/// \param[in] response The response from the Query Engine.
	/// \return The concrete result corresponding to the query.
	virtual Result processResponse(
		const You::QueryEngine::Response& response) = 0;

private:
	/// The query object which we are encapsulating.
	std::unique_ptr<You::QueryEngine::Query> query;
};

}  // namespace Internal
}  // namespace Controller
}  // namespace You

#endif // YOU_CONTROLLER_INTERNAL_QUERY_EXECUTOR_H_

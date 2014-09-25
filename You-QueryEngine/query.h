/// \file api.h
/// Defines the base class for queries.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_QUERY_H_
#define YOU_QUERYENGINE_QUERY_H_

#include "api.h"

/// TODO(evansb) Wait until Diga finishes Transaction.
class Operation {
};

namespace You {
namespace QueryEngine {
class Query {
	/// Execute the query.
	virtual Response execute() = 0;

	/// Convert the query to several lower level
	/// operations defined in data store engine.
	virtual operator Operation() = 0;
};
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_QUERY_H_

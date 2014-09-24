/// \file query.h
/// Defines the API for Query Engine.
/// \author a0112054y

#pragma once
#ifndef YOU_QUERYENGINE_QUERY_H_
#define YOU_QUERYENGINE_QUERY_H_

#include <memory>
#include "internal/schema.h"

namespace You {
namespace QueryEngine {

typedef Internal::Query::Query Query;

std::shared_ptr<Query> AddTask();

/// Execute a query from the parser.
std::wstring executeQuery(std::shared_ptr<Query> query);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_QUERY_H_

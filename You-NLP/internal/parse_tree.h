#pragma once
#ifndef YOU_NLP_INTERNAL_PARSE_TREE_H_
#define YOU_NLP_INTERNAL_PARSE_TREE_H_

#include <boost/variant.hpp>

#include "parse_tree/add_query.h"

namespace You {
namespace NLP {
namespace Internal {

/// Defines the synthesised value type of a query.
typedef boost::variant<ADD_QUERY> QUERY;

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_PARSE_TREE_H_

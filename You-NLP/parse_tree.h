//@author A0097630B
#pragma once
#ifndef YOU_NLP_PARSE_TREE_H_
#define YOU_NLP_PARSE_TREE_H_

#include <boost/variant.hpp>

#include "parse_tree/add_query.h"
#include "parse_tree/edit_query.h"
#include "parse_tree/delete_query.h"
#include "parse_tree/task_priority.h"

namespace You {
namespace NLP {

/// Defines the synthesised value type of a query.
typedef boost::variant<
	ADD_QUERY,
	EDIT_QUERY,
	DELETE_QUERY
> QUERY;

}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_PARSE_TREE_H_

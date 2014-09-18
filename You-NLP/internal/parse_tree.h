#pragma once
#ifndef YOU_NLP_INTERNAL_PARSE_TREE_H_
#define YOU_NLP_INTERNAL_PARSE_TREE_H_

#include <string>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/variant.hpp>

namespace You {
namespace NLP {
namespace Internal {

/// Defines the synthesised value type of an add query.
struct ADD_QUERY {
	/// The description of the task.
	std::wstring description;
	
	/// The due date of the task.
	std::wstring due;
};

/// Defines the synthesised value type of a query.
typedef boost::variant<ADD_QUERY> QUERY;

}  // namespace Internal
}  // namespace NLP
}  // namespace You

/// \section Boost::Fusion adapters 

BOOST_FUSION_ADAPT_STRUCT(
	You::NLP::Internal::ADD_QUERY,
	(std::wstring, description)
	(std::wstring, due)
)

#endif  // YOU_NLP_INTERNAL_PARSE_TREE_H_

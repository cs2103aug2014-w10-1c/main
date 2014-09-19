#pragma once
#ifndef YOU_NLP_INTERNAL_PARSE_TREE_ADD_QUERY_H_
#define YOU_NLP_INTERNAL_PARSE_TREE_ADD_QUERY_H_

#include <string>
#include <boost/fusion/adapted/struct.hpp>

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

}  // namespace Internal
}  // namespace NLP
}  // namespace You

BOOST_FUSION_ADAPT_STRUCT(
	You::NLP::Internal::ADD_QUERY,
	(std::wstring, description)
	(std::wstring, due)
)

#endif

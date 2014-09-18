#pragma once
#ifndef YOU_NLP_INTERNAL_PARSE_TREE_H_
#define YOU_NLP_INTERNAL_PARSE_TREE_H_

#include <string>
#include <boost/fusion/adapted/struct.hpp>
#include <boost/variant.hpp>

namespace You {
namespace NLP {
namespace Internal {

struct AddQuery {
	std::wstring description;
	std::wstring due;
};

typedef boost::variant<AddQuery> Query;

}  // namespace Internal
}  // namespace NLP
}  // namespace You

BOOST_FUSION_ADAPT_STRUCT(
	You::NLP::Internal::AddQuery,
	(std::wstring, description)
	(std::wstring, due)
)

#endif  // YOU_NLP_INTERNAL_PARSE_TREE_H_

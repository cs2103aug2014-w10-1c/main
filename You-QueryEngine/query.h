#pragma once
#ifndef YOU_QUERYENGINE_QUERY_H_
#define YOU_QUERYENGINE_QUERY_H_

#include <string>
#include <boost/variant.hpp>

namespace You {
namespace QueryEngine {

/// Raw queries returned from NLP parser that will be
/// converted to real queries.
namespace Raw {

struct ADD_QUERY {
	std::wstring description;
	std::wstring deadline;
	std::wstring priority;
	std::wstring dependencies;
};

struct READ_QUERY {
	std::wstring taskID;
};

struct DELETE_QUERY {
	std::wstring taskID;
};

struct FIND_QUERY {
	std::wstring searchKeyword;
};

struct EDIT_QUERY {
	std::wstring taskID;
	std::wstring description;
	std::wstring deadline;
	std::wstring priority;
	std::wstring dependencies;
};

}  // namespace Raw

typedef boost::variant
	< Raw::ADD_QUERY
	, Raw::READ_QUERY
	, Raw::DELETE_QUERY
	, Raw::FIND_QUERY
	, Raw::EDIT_QUERY
	> QUERY;

/// This is all the parser need to call.
std::wstring executeQuery(QUERY query);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_QUERY_H_

#pragma once
#ifndef YOU_QUERYENGINE_QUERY_H_
#define YOU_QUERYENGINE_QUERY_H_

#include <string>

namespace You {
namespace QueryEngine {

/// Defines schema for raw queries from NLP parser that will be
/// converted to real queries, executed, and converted back to string
/// to be displayed in the GUI.
namespace Raw {

struct Query {};

struct ADD_QUERY : public Query {
	std::wstring description;
	std::wstring deadline;
	std::wstring priority;
	std::wstring dependencies;
};

struct READ_QUERY : public Query {
	std::wstring taskID;
};

struct DELETE_QUERY : public Query {
	std::wstring taskID;
};

struct FIND_QUERY : public Query {
	std::wstring searchKeyword;
};

}  // namespace Raw

/// This is all the parser need to call.
std::wstring executeQuery(Raw::Query query);

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_QUERY_H_

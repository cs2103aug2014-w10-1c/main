/// \file You-QueryEngine/internal/filter.h
/// Defines the base class for filters.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_FILTER_H_
#define YOU_QUERYENGINE_FILTER_H_

#include <stdexcept>
#include <functional>
#include "task_model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class FilterTests; }

/// Base task filterer class
/// New instances of the filter can only be create
/// by using or combining common filters provided.
class Filter {
public:
	/// Type of the predicate function using in Filter
	typedef std::function<bool(const Task&)> FFilter;

	/// \name Common Filters
	/// @{
	static Filter anyTask();
	static Filter idIsIn(std::vector<Task::ID> taskIDs);
	/// @}

	/// Compose an AND operation with another filter
	/// \param[in] filter Filter object to combine with.
	/// \returns Own reference.
	Filter& operator&&(const Filter& filter);

	/// Compose an OR operation with another filter
	/// \param[in] filter Filter object to combine with.
	/// \returns Own reference
	Filter& operator||(const Filter& filter);

	/// Negate a filter
	/// \param[in] filter Filter object to be negated.
	Filter& operator!(void);

	/// Compose an AND NOT operation with another filter
	/// \param[in] filter Filter object to combine with.
	/// \returns Own reference
	const Filter butNot(const Filter& filter);

	/// Implicitly cast to its containing predicate function.
	operator FFilter() const;

	/// Filter can be applied directly
	bool operator()(const Task&) const;

	Filter(const Filter& filter) : ffilter(filter.ffilter) {}

private:
	explicit Filter(const FFilter& ffilter) : ffilter(ffilter) {}

	static FFilter AND(const FFilter& f, const FFilter& g);
	static FFilter OR(const FFilter& f, const FFilter& g);
	static FFilter NOT(const FFilter& f);

	FFilter ffilter;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_FILTER_H_

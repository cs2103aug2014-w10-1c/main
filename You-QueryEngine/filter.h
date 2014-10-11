/// \file You-QueryEngine/filter.h
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

/// Base class for filter object.
/// New instances of the filter can be created by
/// NLP by combining common filters provided.
class Filter {
public:
	/// Type of the predicate function using in Filter
	typedef std::function<bool(const Task&)> FFilter;

	/// Filter any task.
	static Filter anyTask();
	/// Filter tasks that id is in a list.
	static Filter idIsIn(std::vector<Task::ID> taskIDs);
	/// Filter completed tasks.
	static Filter completed();

	/// Compose using AND operation with another filter
	/// \param[in] filter Filter object to combine with.
	/// \returns Own reference.
	Filter& operator&&(const Filter& filter);

	/// Compose using OR operation with another filter
	/// \param[in] filter Filter object to combine with.
	/// \returns Own reference
	Filter& operator||(const Filter& filter);

	/// Invert the filter using NOT.
	Filter& operator!(void);

	/// Shortcut for AND NOT operation with another filter.
	/// \param[in] filter Filter object to combine with.
	/// \returns Own reference
	const Filter butNot(const Filter& filter);

	/// Implicitly cast a filter to its predicate function.
	operator FFilter() const;

	/// Filter can be applied directly
	bool operator()(const Task&) const;

	/// Copy construct a filter.
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

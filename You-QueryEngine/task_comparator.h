/// \file You-QueryEngine/comparator.h
/// Defines the base class for sorting comparators.
/// \author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_COMPARATOR_H_
#define YOU_QUERYENGINE_COMPARATOR_H_

#include <stdexcept>
#include <functional>
#include "task_model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class ComparatorTests; }

/// Base class for task comparators.
/// Needed by Controller to construct GetTask query.
class Comparator {
public:
	/// Enum class for comparison result.
	enum class ComparisonResult { LT, GT, EQ };

	/// Type of the inner comparator function.
	typedef std::function<ComparisonResult(const Task&, const Task&)>
		ComparatorFunc;

	/// Constructor using binary function.
	Comparator(const ComparatorFunc& func);

	/// Cast to binary comparison function used by std::sort and
	/// the others.
	/// \note Default is ascending
	bool operator()(const Task& lhs, const Task& rhs) const;

	/// Set the comparator function inside to compare
	/// ascendingly.
	Comparator& ascending();

	/// Set the comparator function inside to compare
	/// descendingly.
	Comparator& descending();

	/// Combine with a second comparator
	/// to be tested with it in case it is equal.
	Comparator& operator&&(const Comparator& rhs);

private:
	ComparatorFunc negate(const ComparatorFunc& comp);
	void negateAllComparators();
	std::vector<const ComparatorFunc> comparators;
	bool isAscending = false;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_COMPARATOR_H_

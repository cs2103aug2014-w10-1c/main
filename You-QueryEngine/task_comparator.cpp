#include "stdafx.h"
#include "task_comparator.h"

namespace You {
namespace QueryEngine {

Comparator::Comparator(const ComparatorFunc& func) {
	comparators.push_back(func);
}

bool Comparator::operator() (const Task& lhs, const Task& rhs) const {
	for (auto comparator = comparators.cbegin();
		 comparator != comparators.cend();
		 ++comparator) {
		ComparisonResult result = (*comparator)(lhs, rhs);
		if (result != ComparisonResult::EQ) {
			return result == ComparisonResult::GT;
		} else {
			continue;
		}
	}
	/// If identical, assume greater than.
	return true;
}

void Comparator::negateAllComparators() {
	std::vector<const ComparatorFunc> newComparators;
	std::for_each(comparators.cbegin(), comparators.cend(),
		[&] (const ComparatorFunc& func) {
			newComparators.push_back(this->negate(func));
		}
	);
	comparators = newComparators;
}

Comparator& Comparator::ascending() {
	if (!isAscending) {
		negateAllComparators();
	}
	isAscending = true;
	return *this;
}

Comparator& Comparator::descending() {
	if (isAscending) {
		negateAllComparators();
	}
	isAscending = false;
	return *this;
}

Comparator& Comparator::operator&&(const Comparator& rhs) {
	comparators.insert(comparators.end(),
		rhs.comparators.begin(), rhs.comparators.end());
	return *this;
}

Comparator::ComparatorFunc Comparator::negate(const ComparatorFunc& comp) {
	return [=] (const Task& lhs, const Task& rhs) {
		ComparisonResult result = comp(lhs, rhs);
		if (result == ComparisonResult::LT) {
			return ComparisonResult::GT;
		} else if (result == ComparisonResult::GT) {
			return ComparisonResult::LT;
		} else {
			return ComparisonResult::EQ;
		}
	};
}

}  // namespace QueryEngine
}  // namespace You

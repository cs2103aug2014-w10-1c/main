#include "stdafx.h"
#include "comparator.h"

namespace You {
namespace QueryEngine {

Comparator Comparator::notSorted() {
	return Comparator([](const Task&, const Task&) {
		return ComparisonResult::GT;
	});
}

Comparator Comparator::byDescription() {
	return byApplying<Task::Description>([](const Task& task) {
		return task.getDescription();
	});
}

Comparator Comparator::byDeadline() {
	return byApplying<Task::Time>([](const Task& task) {
		return task.getDeadline();
	});
}

Comparator Comparator::byDependenciesCount() {
	return byApplying<int>([](const Task& task) {
		return task.getDependencies().size();
	});
}

Comparator Comparator::byPriority() {
	return byApplying<int>([](const Task& task) {
		if (task.getPriority() == Task::Priority::NORMAL) {
			return 1;
		} else {
			return 0;
		}
	});
}

Comparator::Comparator(const ComparatorFunc& func) {
	comparators.push_back(func);
}

bool Comparator::operator() (const Task& lhs, const Task& rhs) const {
	for (auto comparator = comparators.cbegin();
		 comparator != comparators.cend();
		 ++comparator) {
		ComparisonResult result = (*comparator)(lhs, rhs);
		if (result != ComparisonResult::EQ) {
			return result == ComparisonResult::LT;
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

//@author A0112054Y
#include "stdafx.h"
#include "comparator.h"
#include "filter.h"
#include "internal/model.h"

namespace You {
namespace QueryEngine {

Comparator Comparator::notSorted() {
	return Comparator([](const Task&, const Task&) {
		return ComparisonResult::EQ;
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
			return 0;
		} else {
			return 1;
		}
	});
}

Comparator Comparator::byRelationship(Task::ID id) {
	return byApplying<int>([id] (const Task& task) {
		auto theTask = Internal::State::get().graph().getTask(id);
		if (task.isTopLevel()) {
			return 4;
		} else if (Filter::isChildOf(id)(task)) {
			return 3;
		} else if (Filter::isDependedBy(id)(task)) {
			return 2;
		} else if (Filter::isDependOn(id)(task)) {
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
	ComparisonResult result = ComparisonResult::EQ;
	for (const auto& comparator : comparators) {
		result = comparator(lhs, rhs);
		if (result != ComparisonResult::EQ) {
			break;
		} else {
			continue;
		}
	}

	// If identical, it is NOT less than.
	return result == ComparisonResult::LT;
}

void Comparator::negateAllComparators() {
	std::vector<const ComparatorFunc> newComparators;
	for (const auto& func : comparators) {
		newComparators.push_back(this->negate(func));
	}
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
		begin(rhs.comparators), end(rhs.comparators));
	return *this;
}

Comparator::ComparatorFunc Comparator::negate(const ComparatorFunc& comp) {
	return [comp] (const Task& lhs, const Task& rhs) {
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

/// \file You-QueryEngine/comparator.h
/// Defines the base class and constructors for sorting comparators.
//@author A0112054Y

#pragma once
#ifndef YOU_QUERYENGINE_COMPARATOR_H_
#define YOU_QUERYENGINE_COMPARATOR_H_

#include <stdexcept>
#include <functional>
#include "task_model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests { class ComparatorTests; }

/// Base class for binary comparators.
/// Comparators are helper objects used to sort any object based on
/// a certain properties of the object.
/// Comparators can be combined by using the && operator.
/// \see \ref You::QueryEngine::GetTask
class Comparator {
	friend class You::QueryEngine::UnitTests::ComparatorTests;
public:
	/// \name Commonly used comparators
	/// @{

	/// Do not sort.
	static Comparator notSorted();

	/// Sort by time the task is created
	static Comparator byTimeCreated();

	/// Sort lexicographically by description.
	static Comparator byDescription();

	/// Sort by deadline.
	static Comparator byStartTime();

	/// Sort by deadline.
	static Comparator byDeadline();

	/// Sort by the number of dependencies.
	static Comparator byDependenciesCount();

	/// Sort by priority, normal goes first.
	static Comparator byPriority();

	/// Sort nicely by relationship
	static Comparator byRelationship(Task::ID id);
	/// @}

public:
	/// Enumerated type for comparison result.
	/// When comparing two tasks x and y, tasks x goes first
	/// if the result is LT (conversely, GT). If the result is EQ,
	/// continue to use the second comparator.
	enum class ComparisonResult { LT, GT, EQ };

	/// Type of the binary function underneath the comparator.
	typedef std::function<ComparisonResult(const Task&, const Task&)>
		ComparatorFunc;

	/// Construct a comparator from using binary function.
	Comparator(const ComparatorFunc& func);  // NOLINT

	/// Return whether this is the default comparator.
	inline bool isDefault() const { return isDefaultComparator; }

	/// Cast to binary comparison function used by std::sort and the others.
	/// \note Default comparison method is ascending.
	/// \param [in] lhs The first task to be compared.
	/// \param [in] lhs The second task to be compared.
	/// \return True if the comparison result is less than.
	bool operator()(const Task& lhs, const Task& rhs) const;

	/// Set the comparator function underneath to compare ascendingly.
	/// \return Self reference after modification.
	Comparator& ascending();

	/// Set the comparator function underneath to compare descendingly.
	/// \return Self reference after modification.
	Comparator& descending();

	/// Combine this comparator with another comparator
	/// \param [in] rhs Another comparator to combine with.
	Comparator& operator&&(const Comparator& rhs);

private:
	/// Helper function to create a comparator for two task objects
	/// by previously applying a unary function to both task objects.
	/// \param [in] func The unary function to be pre-applied.
	template <class T>
	static Comparator byApplying(std::function<T(const Task&)> func) {
		return Comparator([func] (const Task& lhs, const Task& rhs) {
			if (func(lhs) < func(rhs)) {
				return ComparisonResult::LT;
			} else if (func(lhs) > func(rhs)) {
				return ComparisonResult::GT;
			} else {
				return ComparisonResult::EQ;
			}
		});
	}

private:
	ComparatorFunc negate(const ComparatorFunc& comp);
	void negateAllComparators();
	std::vector<const ComparatorFunc> comparators;
	bool isAscending = true;
	bool isDefaultComparator = false;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_COMPARATOR_H_

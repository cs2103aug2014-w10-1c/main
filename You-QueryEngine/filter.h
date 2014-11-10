/// \file You-QueryEngine/filter.h
/// Defines the base class and common constructors. for filters.
//@author A0112054Y

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

	/// \name Commonly used filters
	/// @{
	/// Filter any task.
	static Filter anyTask();

	/// Filter tasks that id is in a list.
	static Filter idIsIn(std::vector<Task::ID> taskIDs);

	/// Filter completed tasks.
	static Filter completed();

	/// Filter tasks that depends on the task.
	static Filter dependsOn(const Task::ID id);

	/// Filter tasks with high priority.
	static Filter highPriority();

	/// Filter tasks with normal priority.
	static Filter normalPriority();

	/// Filter tasks that is overdue this year.
	static Filter overdue();

	/// Filter tasks that due this year.
	static Filter dueThisYear();

	/// Filter tasks with high priority.
	static Filter dueThisMonth();

	/// Filter tasks that due this month.
	static Filter dueToday();

	/// Filter tasks that due this month.
	static Filter dueThisWeek();

	/// Filter tasks that does not have deadline set (i.e never)
	static Filter dueNever();

	/// Filter tasks that due before a date
	/// Date is assumed valid
	static Filter dueBefore(std::int16_t year, std::int16_t month,
		std::int16_t day, std::int16_t hour, std::int16_t minute,
		std::int16_t seconds);
	/// @}

	/// Filter task that has relationship with a task.
	static Filter Filter::isRelatedTo(Task::ID id);

	/// Filter task that match the parent of the task.
	static Filter isParentOf(Task::ID id);

	/// Filter the subtasks.
	static Filter isChildOf(Task::ID id);

	/// Filter the depedencies.
	static Filter isDependedBy(Task::ID id);

	/// Filter the depedendants.
	static Filter isDependOn(Task::ID id);

	/// Filter the descendants.
	static Filter isDescendantOf(Task::ID id);

	/// Construct a filter from a filtering function.
	/// \see FFilter
	explicit Filter(const FFilter& ffilter)
	: ffilter(ffilter) {}

	/// Copy construct a filter.
	Filter(const Filter& filter)
	: ffilter(filter.ffilter) {}

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

	/// Functor for filter to task.
	/// Filter can be applied directly
	bool operator()(const Task&) const;

private:
	static FFilter AND(const FFilter& f, const FFilter& g);
	static FFilter OR(const FFilter& f, const FFilter& g);
	static FFilter NOT(const FFilter& f);

	struct Time {
		std::int16_t year;
		std::int16_t month;
		std::int16_t day;
		std::int16_t hour;
		std::int16_t minute;
	};

	static Time now();

	FFilter ffilter;
};

}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_FILTER_H_

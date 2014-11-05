//@author A0112054Y
#include "stdafx.h"
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "internal/model/state.h"
#include "filter.h"

namespace You {
namespace QueryEngine {

namespace {
	using boost::posix_time::ptime;
	using boost::posix_time::time_duration;
	using boost::gregorian::date;
	using boost::gregorian::to_tm;
	using boost::gregorian::day_clock;
	using boost::gregorian::weeks;
}

#pragma region Common Filters
Filter Filter::anyTask() {
	return Filter([] (const Task& task) {
		return true;
	});
}

Filter Filter::idIsIn(std::vector<Task::ID> taskIDs) {
	return Filter([taskIDs] (const Task& task) {
		return std::find(begin(taskIDs), end(taskIDs),
			task.getID()) != taskIDs.end();
	});
}

Filter Filter::completed() {
	return Filter([] (const Task& task) {
		return task.isCompleted();
	});
}

Filter Filter::dependsOn(const Task::ID id) {
	return Filter([id] (const Task& task) {
		return task.isDependOn(id);
	});
}

Filter Filter::highPriority() {
	return Filter([] (const Task& task) {
		return task.getPriority() == Task::Priority::HIGH;
	});
}

Filter Filter::normalPriority() {
	return Filter([] (const Task& task) {
		return task.getPriority() == Task::Priority::NORMAL;
	});
}

Filter Filter::overdue() {
	return Filter([] (const Task& task) {
		return task.getDeadline().date() < day_clock::local_day();
	});
}

Filter Filter::dueThisYear() {
	return !completed() && Filter([] (const Task& task) {
		return task.getDeadline().date().year()
			== now().year;
	});
}

Filter Filter::dueThisMonth() {
	return dueThisYear() && Filter([] (const Task& task) {
		return task.getDeadline().date().month()
			== now().month;
	});
}

Filter Filter::dueToday() {
	return dueThisMonth() && Filter([] (const Task& task) {
		return task.getDeadline().date().day()
			== now().day;
	});
}

Filter Filter::dueThisWeek() {
	return dueThisMonth() && Filter([] (const Task& task) {
		return (task.getDeadline().date() - day_clock::local_day()) <
			weeks(1);
	});
}

Filter Filter::dueNever() {
	return Filter([] (const Task& task) {
		return task.getDeadline() == Task::NEVER;
	});
}

Filter Filter::dueBefore(std::int16_t year, std::int16_t month,
	std::int16_t day, std::int16_t hour, std::int16_t minute,
	std::int16_t seconds) {
	auto due = ptime(date(year, month, day),
			time_duration(hour, minute, seconds));
	return Filter([due] (const Task& task) {
		return task.getDeadline() < due;
	});
}

Filter Filter::isChildOf(Task::ID id) {
	return Filter([id] (const Task& task) {
		return !task.isTopLevel() &&
			task.getParent() == id;
	});
}

Filter Filter::isParentOf(Task::ID id) {
	auto theTask = Internal::State::get().sgraph().getTask(id);
	return Filter([id, theTask] (const Task& task) {
		return theTask.getParent() == task.getID();
	});
}

Filter Filter::isDependOn(Task::ID id) {
	return Filter([id] (const Task& task) {
		return task.isDependOn(id);
	});
}

Filter Filter::isDependedBy(Task::ID id) {
	auto theTask = Internal::State::get().sgraph().getTask(id);
	return Filter([id, theTask] (const Task& task) {
		return theTask.isDependOn(task.getID());
	});
}

Filter Filter::isRelatedTo(Task::ID id) {
	return Filter([=](const Task& task) {
		return (Filter::isChildOf(id)
			|| Filter::isParentOf(id)
			|| Filter::isDependOn(id)
			|| Filter::isDependedBy(id))(task);
	});
 }

Filter Filter::isDescendantOf(Task::ID id) {
	auto theTask = Internal::State::get().graph().getTask(id);
	return Filter([=] (const Task& task) {
		bool cond = (isChildOf(id))(task);
		for (auto child : theTask.getSubtasks()) {
			cond = cond || (isDescendantOf(child))(task);
		}
		return cond;
	});
}

#pragma endregion

Filter& Filter::operator&&(const Filter& filter) {
	ffilter = AND(ffilter, filter.ffilter);
	return *this;
}

Filter& Filter::operator||(const Filter& filter) {
	ffilter = OR(ffilter, filter.ffilter);
	return *this;
}

Filter& Filter::operator!(void) {
	ffilter = NOT(ffilter);
	return *this;
}

bool Filter::operator()(const Task& task) const {
	return ffilter(task);
}

Filter::FFilter Filter::AND(const FFilter& f, const FFilter& g) {
	return [f, g] (FFilter::argument_type x) {
		return f(x) && g(x);
	};
}

Filter::FFilter Filter::OR(const FFilter& f, const FFilter& g) {
	return [f, g] (FFilter::argument_type x) {
		return f(x) || g(x);
	};
}

Filter::FFilter Filter::NOT(const FFilter& f) {
	return [f] (FFilter::argument_type x) {
		return !f(x);
	};
}

Filter::operator FFilter() const {
	return this->ffilter;
}

Filter::Time Filter::now() {
	auto date = day_clock::local_day();
	auto tm = to_tm(date);
	return {
		date.year(),
		date.month(),
		date.day(),
		tm.tm_hour,
		tm.tm_min
	};
}

}  // namespace QueryEngine
}  // namespace You

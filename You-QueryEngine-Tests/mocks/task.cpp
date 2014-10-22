#include "stdafx.h"

#include "../You-QueryEngine/internal/controller.h"
#include "../You-QueryEngine/internal/model.h"

#include "task.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

namespace {
	using boost::gregorian::greg_month;
	using boost::gregorian::greg_year;
	using boost::posix_time::ptime;
	using boost::posix_time::time_duration;
	using boost::posix_time::to_tm;
	using boost::posix_time::hours;
	using boost::gregorian::date;
	using boost::gregorian::days;
	using boost::gregorian::day_clock;

	using Task = You::QueryEngine::Task;
	using Controller = You::QueryEngine::Internal::Controller;
}  // namespace

Task TASK_WITH_ID_1() {
	return Controller::Builder::get()
		.id(1)
		.description(L"Hello World");
}

Task TASK_WITH_ID_2() {
	return Controller::Builder::get()
		.id(2)
		.description(L"Hello World");
}

Task TASK_WITH_ID_3() {
	return Controller::Builder::get()
		.id(3)
		.description(L"Hello World");
}

Task FEED_THE_DOGGY() {
	return Controller::Builder::get().
		id(43L).
		description(L"Feed the doggy");
}

Task FEED_THE_KITTEN() {
	return Controller::Builder::get().
		id(42L).
		description(L"Feed the kitten");
}

Task SUPER_IMPORTANT_TASK() {
	return Controller::Builder::get().
		id(42L).
		description(L"Sleep").
		priority(Task::Priority::HIGH);
}

Task LESS_IMPORTANT_TASK() {
	return Controller::Builder::get().
		id(42L).
		description(L"Save the Earth").
		priority(Task::Priority::NORMAL);
}

Task FOR_TOMMOROW() {
	return Controller::Builder::get().
		id(42L).
		description(L"Save the Earth").
		deadline(ptime(date(2014, 10, 7)));
}

Task FOR_NEXT_WEEK() {
	return Controller::Builder::get().
		id(43L).
		description(L"Save the Earth").
		deadline(ptime(date(2014, 10, 14)));
}

Task DEPENDS_ON_1_2_3() {
	return Controller::Builder::get()
		.id(0)
		.description(L"Hello World")
		.dependencies({ 1, 2, 3 });
}

Task DEPENDS_ON_3_4_5() {
	return Controller::Builder::get()
		.id(0)
		.description(L"Hello World")
		.dependencies({ 3, 4, 5 });
}

Task DEPENDS_ON_NOTHING() {
	return Controller::Builder::get()
		.id(0)
		.description(L"Hello World")
		.dependencies({});
}

Task TEN_DEPENDENCIES() {
	return Controller::Builder::get().
		id(666L).
		description(L"Impossible is nothing").
		dependencies({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });
}

Task NINE_DEPENDENCIES() {
	return Controller::Builder::get().
		id(666L).
		description(L"Impossible is nothing").
		dependencies({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });
}

Task OVERDUE() {
	date now = day_clock::local_day();
	return Controller::Builder::get().
		id(23L).
		description(L"Overdue").
		deadline(ptime(now) - hours(1));
}

Task DUE_THIS_MONTH() {
	date now = day_clock::local_day();
	return Controller::Builder::get().
		id(23L).
		description(L"This Month").
		deadline(ptime(
			date(now.year(), now.month(), 1),
			time_duration(0, 0, 0)));
}

Task DUE_THIS_WEEK() {
	date now = day_clock::local_day();
	ptime due(now + days(1));
	return Controller::Builder::get().
		id(23L).
		description(L"This Week").
		deadline(due);
}

Task DUE_TODAY() {
	date now = day_clock::local_day();
	return Controller::Builder::get().
		id(23L).
		description(L"Today").
		deadline(ptime(now));
}

Task DUE_ON_CHRISTMAS() {
	return Controller::Builder::get().
		id(23L).
		description(L"Christmas").
		deadline(ptime(date(2014, 12, 25),
					   time_duration(0, 0, 0)));
}

Task DUE_BEFORE_CHRISTMAS() {
	return Controller::Builder::get().
		id(23L).
		description(L"Before Christmas").
		deadline(ptime(date(2014, 12, 24),
					   time_duration(0, 0, 0)));
}

Task DUE_AFTER_CHRISTMAS() {
	return Controller::Builder::get().
		id(23L).
		description(L"After Christmas").
		deadline(ptime(date(2014, 12, 26),
					   time_duration(0, 0, 0)));
}

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

#include "task.h"
#include "stdafx.h"

#include "../You-QueryEngine/internal/controller.h"
#include "../You-QueryEngine/internal/model.h"

namespace You {
namespace QueryEngine {
namespace UnitTests {

namespace {
	using boost::gregorian::greg_month;
	using boost::gregorian::greg_year;
	using boost::posix_time::ptime;
	using boost::posix_time::time_duration;
	using boost::gregorian::date;

	using Task = You::QueryEngine::Task;
	using Controller = You::QueryEngine::Internal::Controller;
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

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

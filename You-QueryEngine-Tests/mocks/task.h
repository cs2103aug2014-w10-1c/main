#pragma once
#ifndef YOU_QUERYENGINE_TESTS_MOCKS_TASK_H_
#define YOU_QUERYENGINE_TESTS_MOCKS_TASK_H_

#include "../You-QueryEngine/internal/controller.h"
#include "../You-QueryEngine/internal/model.h"

using boost::gregorian::greg_month;
using boost::gregorian::greg_year;
using boost::posix_time::ptime;
using boost::posix_time::time_duration;
using boost::gregorian::date;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using Task = You::QueryEngine::Task;
using Controller = You::QueryEngine::Internal::Controller;

const Task FEED_THE_DOGGY = Controller::Builder::get().
	id(43L).
	description(L"Feed the doggy");

const Task FEED_THE_KITTEN = Controller::Builder::get().
	id(42L).
	description(L"Feed the kitten");

const Task SUPER_IMPORTANT_TASK = Controller::Builder::get().
	id(42L).
	description(L"Sleep").
	priority(Task::Priority::HIGH);

const Task LESS_IMPORTANT_TASK = Controller::Builder::get().
	id(42L).
	description(L"Save the Earth").
	priority(Task::Priority::NORMAL);

const Task FOR_TOMMOROW = Controller::Builder::get().
	id(42L).
	description(L"Save the Earth").
	deadline(ptime(date(2014, 10, 7)));

const Task FOR_NEXT_WEEK = Controller::Builder::get().
	id(43L).
	description(L"Save the Earth").
	deadline(ptime(date(2014, 10, 14)));

const Task TEN_DEPENDENCIES = Controller::Builder::get().
	id(666L).
	description(L"Impossible is nothing").
	dependencies({ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 });

const Task NINE_DEPENDENCIES = Controller::Builder::get().
	id(666L).
	description(L"Impossible is nothing").
	dependencies({ 1, 2, 3, 4, 5, 6, 7, 8, 9 });

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

#endif  // YOU_QUERYENGINE_TESTS_MOCKS_TASK_H_

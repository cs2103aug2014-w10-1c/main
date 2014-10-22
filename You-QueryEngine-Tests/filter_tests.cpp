#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"
#include "mocks/task.h"
#include "mocks/task_list.h"
#include "internal/model.h"
#include "internal/controller.h"
#include "api.h"
#include "filter.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

namespace {
	using boost::gregorian::date;
	using boost::gregorian::max_date_time;

	using State = You::QueryEngine::Internal::State;
	using Controller = You::QueryEngine::Internal::Controller;
	using F = You::QueryEngine::Filter;
}

TEST_CLASS(FilterTests) {
	static const std::size_t N_TASK = 10;

	TEST_METHOD(implicitConversionFromFilterToLambda) {
		Assert::IsTrue((F::anyTask())(FEED_THE_DOGGY()));
	}

	TEST_METHOD(filterAnyTask) {
		Assert::IsTrue((F::anyTask())(FEED_THE_KITTEN()));
		Assert::IsFalse((!F::anyTask())(FEED_THE_KITTEN()));
	}

	TEST_METHOD(filterIdIsIn) {
		Assert::IsTrue((F::idIsIn({ 1, 2, 3 }))(TASK_WITH_ID_1()));
		Assert::IsFalse((F::idIsIn({ 4, 4, 5, 6 }))(TASK_WITH_ID_1()));
		Assert::IsTrue((F::idIsIn({ 2, 4 }) || F::idIsIn({ 3 , 5}))
			(TASK_WITH_ID_3()));
	}

	TEST_METHOD(filterByDependencies) {
		Assert::IsTrue((F::dependsOn(1))(DEPENDS_ON_1_2_3()));
		Assert::IsTrue((F::dependsOn(3))(DEPENDS_ON_1_2_3()));
		Assert::IsFalse((F::dependsOn(4))(DEPENDS_ON_1_2_3()));
		Assert::IsTrue((F::dependsOn(2) && F::dependsOn(3))
			(DEPENDS_ON_1_2_3()));
		Assert::IsTrue((F::dependsOn(4) || F::dependsOn(3))
			(DEPENDS_ON_1_2_3()));
	}

	TEST_METHOD(filterByPriority) {
		Assert::IsTrue((F::highPriority())(SUPER_IMPORTANT_TASK()));
		Assert::IsTrue((F::normalPriority())(LESS_IMPORTANT_TASK()));
	}

	TEST_METHOD(filterOverdue) {
		Assert::IsTrue((F::overdue())(OVERDUE()));
	}

	TEST_METHOD(filterDueToday) {
		Assert::IsTrue((F::dueToday())(DUE_TODAY()));
	}

	TEST_METHOD(filterDueThisWeek) {
		Assert::IsTrue((F::dueThisWeek())(DUE_THIS_WEEK()));
	}

	TEST_METHOD(filterDueThisMonth) {
		Assert::IsTrue((F::dueThisMonth())(DUE_THIS_MONTH()));
	}

	TEST_METHOD(filterDueNever) {
		Assert::IsTrue((F::dueNever())(FEED_THE_DOGGY()));
	}

	TEST_METHOD(filterBeforeTime) {
		auto beforeChristmas =
			F::dueBefore(2014, 12, 25, 0, 0, 0);
		Assert::IsTrue(beforeChristmas(DUE_BEFORE_CHRISTMAS()));
		Assert::IsFalse(beforeChristmas(DUE_ON_CHRISTMAS()));
		Assert::IsFalse(beforeChristmas(DUE_AFTER_CHRISTMAS()));
	}

	TEST_METHOD(filterGetterLessThan) {
		F lt = F::lessThan(F::descriptionGetter,
			std::wstring(L"ZZZZ"));
		Assert::IsTrue(lt(FEED_THE_DOGGY()));
	}

	TEST_METHOD(filterGetterLessThanOrEqual) {
		F lt = F::lessThan(F::descriptionGetter,
			std::wstring(FEED_THE_DOGGY().getDescription()));
		F eq = F::equal(F::descriptionGetter,
			std::wstring(FEED_THE_DOGGY().getDescription()));
		F ltOrEq = lt || eq;
		Assert::IsTrue(ltOrEq(FEED_THE_DOGGY()));
	}

	TEST_METHOD(filterGetterEqual) {
		F eq = F::equal(F::descriptionGetter,
			std::wstring(FEED_THE_DOGGY().getDescription()));
		Assert::IsTrue(eq(FEED_THE_DOGGY()));
	}

	TEST_METHOD(filterGetterGreaterThan) {
		F gt = F::greaterThan(F::descriptionGetter,
			std::wstring(FEED_THE_DOGGY().getDescription()));
		Assert::IsTrue(gt(FEED_THE_KITTEN()));
	}

	TEST_METHOD(filterGetterGreaterThanOrEqual) {
		F gt = F::greaterThan(F::descriptionGetter,
			std::wstring(FEED_THE_DOGGY().getDescription()));
		F eq = F::equal(F::descriptionGetter,
			std::wstring(FEED_THE_DOGGY().getDescription()));
		F gtOrEq = gt || eq;
		Assert::IsTrue(gtOrEq(FEED_THE_DOGGY()));
	}

	TEST_METHOD(logicalAndTwoFilters) {
		populateStateWithTasks(ID_ONE_TO_FIVE());
		auto alwaysEmpty = F::anyTask() && (!F::anyTask());
		auto result = QueryEngine::executeQuery(
			QueryEngine::GetTask(alwaysEmpty));

		Assert::IsTrue(boost::get<std::vector<Task>>(result).empty());
	}

	TEST_METHOD(logicalOrTwoFilters) {
		populateStateWithTasks(ID_ONE_TO_FIVE());
		auto alwaysEmpty = (!F::anyTask()) || (!F::anyTask());
		auto result = QueryEngine::executeQuery(
			QueryEngine::GetTask(alwaysEmpty));

		Assert::IsTrue(boost::get<std::vector<Task>>(result).empty());
	}

	QueryEngineTests& operator=(const QueryEngineTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

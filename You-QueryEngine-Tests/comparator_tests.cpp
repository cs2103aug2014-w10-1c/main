#include "stdafx.h"
#include "CppUnitTest.h"

#include "common.h"
#include "internal/model.h"
#include "internal/controller.h"
#include "api.h"
#include "comparator.h"

#include "mocks/task.h"
#include "mocks/task_list.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace QueryEngine {
namespace UnitTests {

using boost::gregorian::date;
using boost::gregorian::max_date_time;

using You::QueryEngine::Task;
using You::QueryEngine::Comparator;
using You::QueryEngine::QueryEngine;
using You::QueryEngine::Internal::State;
using You::QueryEngine::Internal::Controller;

TEST_CLASS(ComparatorTests) {
	TEST_METHOD(ascendingIsTheDefault) {
		Assert::IsTrue(Comparator::byDescription().isAscending);
	}

	TEST_METHOD(descendingComparator) {
		Assert::IsFalse(
			Comparator::byDescription().descending().isAscending);
	}
	TEST_METHOD(compareByDescription) {
		Assert::IsFalse((Comparator::byDescription()
			.descending().ascending())
			(FEED_THE_KITTEN(), FEED_THE_DOGGY()));
		Assert::IsTrue((Comparator::byDescription().descending())
			(FEED_THE_KITTEN(), FEED_THE_DOGGY()));
	}

	TEST_METHOD(useComparatorToSort) {
		auto t = fromDescription(TASK_DESCRIPTIONS());
		std::sort(t.begin(), t.end(), Comparator::byDescription());
		Assert::IsTrue(std::is_sorted(t.begin(), t.end(),
			[] (Task& lhs, Task& rhs) {
				return lhs.getDescription() < lhs.getDescription();
			}));
	}

	TEST_METHOD(compareByDeadline) {
		Assert::IsTrue((Comparator::byDeadline())
			(FOR_TOMMOROW(), FOR_NEXT_WEEK()));
		Assert::IsTrue((Comparator::byDescription().descending())
			(FOR_NEXT_WEEK(), FOR_TOMMOROW()));
	}

	TEST_METHOD(combineComparators) {
		auto complex =
			Comparator::byDescription() &&
			(Comparator::byDeadline().descending());
		Assert::IsTrue(complex(FOR_NEXT_WEEK(), FOR_TOMMOROW()));
	}

	TEST_METHOD(compareByPriority) {
		Assert::IsTrue((Comparator::byPriority())
			(SUPER_IMPORTANT_TASK(), LESS_IMPORTANT_TASK()));
	}

	TEST_METHOD(compareByDependencies) {
		Assert::IsTrue((Comparator::byPriority())
			(TEN_DEPENDENCIES(), NINE_DEPENDENCIES()));
	}

	ComparatorTests& operator=(const ComparatorTests&) = delete;
};

}  // namespace UnitTests
}  // namespace QueryEngine
}  // namespace You

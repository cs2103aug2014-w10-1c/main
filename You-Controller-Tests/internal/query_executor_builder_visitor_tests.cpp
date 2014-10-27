//@author A0097630B
#include "stdafx.h"
#include "You-NLP/parse_tree/task_priority.h"
#include "internal/query_executor.h"
#include "internal/query_executor_builder_visitor.h"
#include "exceptions/context_index_out_of_range_exception.h"

#include "../mocks/task_list.h"
#include "../mocks/query.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

std::wstring ToString(You::Controller::Task::Priority priority) {
	return ToString(static_cast<int>(priority));
}

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

namespace You {
namespace Controller {
namespace Internal {
namespace UnitTests {

namespace Mocks {
	// NOLINTNEXTLINE(build/namespaces)
	using namespace You::Controller::UnitTests::Mocks;
}

using Task = You::Controller::Task;
using TaskPriority = You::NLP::TaskPriority;

TEST_CLASS(QueryExecutorBuilderVisitorTests) {
	TEST_METHOD(getsCorrectTypeForAddQueries) {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::ADD_QUERY);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		ADD_RESULT result(
			boost::get<ADD_RESULT>(executor->execute()));

		Assert::AreEqual(
			Mocks::Queries::ADD_QUERY.description,
			result.task.getDescription());
		Assert::AreEqual(
			Task::Priority::NORMAL,
			result.task.getPriority());
		Assert::AreEqual(
			Mocks::Queries::ADD_QUERY.deadline.get(),
			result.task.getDeadline());

		You::NLP::ADD_QUERY queryWithoutDeadline(Mocks::Queries::ADD_QUERY);
		queryWithoutDeadline.deadline =
			You::Utils::Option<boost::posix_time::ptime>();
		query = queryWithoutDeadline;
		executor = boost::apply_visitor(visitor, query);
		result = boost::get<ADD_RESULT>(executor->execute());

		Assert::AreEqual(
			Mocks::Queries::ADD_QUERY.description,
			result.task.getDescription());
		Assert::AreEqual(
			Task::Priority::NORMAL,
			result.task.getPriority());
		Assert::AreEqual(
			Task::DEFAULT_DEADLINE,
			result.task.getDeadline());

		You::NLP::ADD_QUERY queryWithPriority(Mocks::Queries::ADD_QUERY);
		queryWithPriority.priority = TaskPriority::HIGH;
		query = queryWithPriority;
		executor = boost::apply_visitor(visitor, query);
		result = boost::get<ADD_RESULT>(executor->execute());

		Assert::AreEqual(
			Mocks::Queries::ADD_QUERY.description,
			result.task.getDescription());
		Assert::AreEqual(
			Task::Priority::HIGH,
			result.task.getPriority());
		Assert::AreEqual(
			Mocks::Queries::ADD_QUERY.deadline.get(),
			result.task.getDeadline());
	}

	TEST_METHOD(getsCorrectTypeForShowQueries) {
		SHOW_RESULT result(
			runShowQuery(Mocks::Queries::SHOW_QUERY));
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[](const Task& task) {
			return task.getDeadline() >
				boost::get<boost::posix_time::ptime>(
					Mocks::Queries::SHOW_QUERY.predicates[0].value);
		}));
		Assert::IsTrue(
			std::is_sorted(begin(result.tasks), end(result.tasks),
			[](const Task& left, const Task& right) {
			return left.getDeadline() > right.getDeadline();
		}));

		result = runShowQuery(
			You::NLP::SHOW_QUERY {
				{
					{
						You::NLP::TaskField::PRIORITY,
						You::NLP::SHOW_QUERY::Predicate::EQ,
						You::NLP::TaskPriority::NORMAL
					}
				},
				{
					{
						You::NLP::TaskField::DESCRIPTION,
						You::NLP::SHOW_QUERY::Order::ASCENDING
					}
				}
			});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[](const Task& task) {
			return task.getPriority() ==
				You::QueryEngine::Task::Priority::NORMAL;
		}));
		Assert::IsTrue(
			std::is_sorted(begin(result.tasks), end(result.tasks),
			[](const Task& left, const Task& right) {
			return left.getDescription() < right.getDescription();
		}));

		result = runShowQuery(
			You::NLP::SHOW_QUERY {
				{
					{
						You::NLP::TaskField::COMPLETE,
						You::NLP::SHOW_QUERY::Predicate::EQ,
						false
					}
				},
				{
					{
						You::NLP::TaskField::PRIORITY,
						You::NLP::SHOW_QUERY::Order::DESCENDING
					}
				}
			});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[](const Task& task) {
			return !task.isCompleted();
		}));
		Assert::IsTrue(
			std::is_sorted(begin(result.tasks), end(result.tasks),
			[](const Task& left, const Task& right) {
			return left.getPriority() > right.getPriority();
		}));

		appliesCorrectFilters();
	}

	void appliesCorrectFilters() {
		// Test filters more rigourously
		appliesCorrectFilters<You::NLP::TaskField::DESCRIPTION>(
			std::bind(&Task::getDescription, std::placeholders::_1),
			std::wstring(L"meh 1"));

		auto runTime = boost::posix_time::second_clock::local_time();
		appliesCorrectFilters<You::NLP::TaskField::DEADLINE>(
			std::bind(&Task::getDeadline, std::placeholders::_1),
			runTime + boost::posix_time::hours(1));

#if 0
		// TODO(lowjoel): Implement comparators for priorities.
		appliesCorrectFilters<You::NLP::TaskField::PRIORITY>(
			std::bind(&Task::getPriority, std::placeholders::_1),
			You::NLP::TaskPriority::HIGH);
#endif
	}

	/// This is a very useful template function. Because it tests all 7
	/// operators: equality, inequality, less than, less than or equal,
	/// greater than, greater or equal on a type all at once.
	///
	/// \tparam field The field to access
	/// \param getter The getter to access the record in the object.
	/// \param value The value to compare against.
	template<You::NLP::TaskField field, typename TGetter, typename TValue>
	void appliesCorrectFilters(TGetter getter, const TValue& value) {
		SHOW_RESULT result(runShowQuery(You::NLP::SHOW_QUERY {
				{ { field, You::NLP::SHOW_QUERY::Predicate::EQ, value } }, {}
			}));
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[&getter, &value](const Task& task) {
			return getter(task) == value;
		}));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ { field, You::NLP::SHOW_QUERY::Predicate::NOT_EQ, value } }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[&getter, &value](const Task& task) {
			return getter(task) != value;
		}));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ { field, You::NLP::SHOW_QUERY::Predicate::LESS_THAN, value } }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[&getter, &value](const Task& task) {
			return getter(task) < value;
		}));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ {
				field,
				You::NLP::SHOW_QUERY::Predicate::LESS_THAN_EQ,
				value
			} }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[&getter, &value](const Task& task) {
			return getter(task) <= value;
		}));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ {
				field,
				You::NLP::SHOW_QUERY::Predicate::GREATER_THAN,
				value }
			}, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[&getter, &value](const Task& task) {
			return getter(task) > value;
		}));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ {
				field,
				You::NLP::SHOW_QUERY::Predicate::GREATER_THAN_EQ,
				value
			} }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
			[&getter, &value](const Task& task) {
			return getter(task) >= value;
		}));
	}

	SHOW_RESULT runShowQuery(const You::NLP::QUERY& query) {
		Mocks::TaskList taskList(5);
		QueryExecutorBuilderVisitor visitor(taskList);

		std::unique_ptr<QueryExecutor> executor =
			boost::apply_visitor(visitor, query);
		return boost::get<SHOW_RESULT>(executor->execute());
	}

	TEST_METHOD(getsCorrectTypeForEditQueries) {
		getsCorrectTypeForEditQueries1();
		getsCorrectTypeForEditQueries2();
		getsCorrectTypeForEditQueries3();
		getsCorrectTypeForEditQueries4();
	}

	void getsCorrectTypeForEditQueries1() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::EDIT_QUERY);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		EDIT_RESULT result(
			boost::get<EDIT_RESULT>(executor->execute()));

		Assert::AreEqual(taskList.front().getID(),
			result.task.getID());
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.description.get(),
			result.task.getDescription());
		Assert::AreEqual(Controller::nlpToQueryEnginePriority(
			Mocks::Queries::EDIT_QUERY.priority.get()),
			result.task.getPriority());
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.deadline.get(),
			result.task.getDeadline());
	}

	void getsCorrectTypeForEditQueries2() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2(Mocks::Queries::EDIT_QUERY);
		query2.deadline =
			You::Utils::Option<boost::posix_time::ptime>();
		You::NLP::QUERY query(query2);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		EDIT_RESULT result(
			boost::get<EDIT_RESULT>(executor->execute()));

		Assert::AreEqual(taskList.front().getID(),
			result.task.getID());
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.description.get(),
			result.task.getDescription());
		Assert::AreEqual(Controller::nlpToQueryEnginePriority(
			Mocks::Queries::EDIT_QUERY.priority.get()),
			result.task.getPriority());
		Assert::AreEqual(taskList.front().getDeadline(),
			result.task.getDeadline());
	}

	void getsCorrectTypeForEditQueries3() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2(Mocks::Queries::EDIT_QUERY);
		query2.description =
			You::Utils::Option<std::wstring>();
		You::NLP::QUERY query(query2);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		EDIT_RESULT result(
			boost::get<EDIT_RESULT>(executor->execute()));

		Assert::AreEqual(taskList.front().getID(),
			result.task.getID());
		Assert::AreEqual(taskList.front().getDescription(),
			result.task.getDescription());
		Assert::AreEqual(Controller::nlpToQueryEnginePriority(
			Mocks::Queries::EDIT_QUERY.priority.get()),
			result.task.getPriority());
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.deadline.get(),
			result.task.getDeadline());
	}

	void getsCorrectTypeForEditQueries4() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2(Mocks::Queries::EDIT_QUERY);
		query2.priority =
			You::Utils::Option<You::NLP::TaskPriority>();
		You::NLP::QUERY query(query2);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		EDIT_RESULT result(
			boost::get<EDIT_RESULT>(executor->execute()));

		Assert::AreEqual(taskList.front().getID(),
			result.task.getID());
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.description.get(),
			result.task.getDescription());
		Assert::AreEqual(taskList.front().getPriority(),
			result.task.getPriority());
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.deadline.get(),
			result.task.getDeadline());
	}

	TEST_METHOD(editQueriesOutOfBoundsThrowsContextOutOfRange) {
		Mocks::TaskList taskList(0);
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::EDIT_QUERY);
		Assert::ExpectException<ContextIndexOutOfRangeException>([&]() {
			boost::apply_visitor(visitor, query);
		});
	}

	TEST_METHOD(getsCorrectTypeForDeleteQueries) {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::DELETE_QUERY);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		DELETE_RESULT result(
			boost::get<DELETE_RESULT>(executor->execute()));

		Assert::AreEqual(taskList.front().getID(),
			result.task);
	}

	TEST_METHOD(deleteQueriesOutOfBoundsThrowsContextOutOfRange) {
		Mocks::TaskList taskList(0);
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::DELETE_QUERY);
		Assert::ExpectException<ContextIndexOutOfRangeException>([&]() {
			boost::apply_visitor(visitor, query);
		});
	}

	TEST_METHOD(getsCorrectTypeForUndoQueries) {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::UNDO_QUERY);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		UNDO_RESULT result(
			boost::get<UNDO_RESULT>(executor->execute()));

		// TODO(lowjoel): test for..?
	}
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace Controller
}  // namespace You

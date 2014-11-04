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
		queryWithoutDeadline.deadline = boost::none;
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
			std::bind(
				std::equal_to<You::QueryEngine::Task::Priority>(),
				std::bind(&Task::getPriority, std::placeholders::_1),
				You::QueryEngine::Task::Priority::NORMAL)));
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
				std::bind(
					std::not_equal_to<bool>(),
					std::bind(&Task::isCompleted, std::placeholders::_1),
					true)));
		Assert::IsTrue(
			std::is_sorted(begin(result.tasks), end(result.tasks),
				std::bind(
					std::greater<Task::Priority>(),
					std::bind(&Task::getPriority, std::placeholders::_1),
					std::bind(&Task::getPriority, std::placeholders::_1))));

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

		appliesCorrectFilters<You::NLP::TaskField::COMPLETE>(
			std::bind(&Task::isCompleted, std::placeholders::_1),
			true);

		// TODO(lowjoel): Implement comparators for priorities.
		appliesCorrectFilters<You::NLP::TaskField::PRIORITY>(
			[](const Task& task) {
				return Controller::queryEngineToNlpPriority(
					task.getPriority());
			},
			You::NLP::TaskPriority::HIGH);
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
				std::bind(
					std::equal_to<TValue>(),
					std::bind(getter, std::placeholders::_1),
					value)));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ { field, You::NLP::SHOW_QUERY::Predicate::NOT_EQ, value } }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
				std::bind(
					std::not_equal_to<TValue>(),
					std::bind(getter, std::placeholders::_1),
					value)));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ { field, You::NLP::SHOW_QUERY::Predicate::LESS_THAN, value } }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
				std::bind(
					std::less<TValue>(),
					std::bind(getter, std::placeholders::_1),
					value)));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ {
				field,
				You::NLP::SHOW_QUERY::Predicate::LESS_THAN_EQ,
				value
			} }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
				std::bind(
					std::less_equal<TValue>(),
					std::bind(getter, std::placeholders::_1),
					value)));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ {
				field,
				You::NLP::SHOW_QUERY::Predicate::GREATER_THAN,
				value }
			}, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
				std::bind(
					std::greater<TValue>(),
					std::bind(getter, std::placeholders::_1),
					value)));

		result = runShowQuery(You::NLP::SHOW_QUERY {
			{ {
				field,
				You::NLP::SHOW_QUERY::Predicate::GREATER_THAN_EQ,
				value
			} }, {}
		});
		Assert::IsTrue(
			std::all_of(begin(result.tasks), end(result.tasks),
				std::bind(
					std::greater_equal<TValue>(),
					std::bind(getter, std::placeholders::_1),
					value)));
	}

	SHOW_RESULT runShowQuery(const You::NLP::QUERY& query) {
		Mocks::TaskList taskList(5);
		QueryExecutorBuilderVisitor visitor(taskList);

		std::unique_ptr<QueryExecutor> executor =
			boost::apply_visitor(visitor, query);
		return boost::get<SHOW_RESULT>(executor->execute());
	}

	TEST_METHOD(getsCorrectTypeForEditQueries) {
		// TODO(lowjoel): Remove copy-paste.
		getsCorrectTypeForEditQueries1();
		getsCorrectTypeForEditQueries2();
		getsCorrectTypeForEditQueries3();
		getsCorrectTypeForEditQueries4();
		getsCorrectTypeForEditQueries5();
		getsCorrectTypeForEditQueries6();
		getsCorrectTypeForEditQueries7();
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
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.complete.get(),
			result.task.isCompleted());
		Assert::IsTrue(result.task.getSubtasks().empty());
		Assert::IsTrue(result.task.getDependencies().empty());
	}

	void getsCorrectTypeForEditQueries2() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2(Mocks::Queries::EDIT_QUERY);
		query2.deadline = boost::none;
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
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.complete.get(),
			result.task.isCompleted());
		Assert::IsTrue(result.task.getSubtasks().empty());
		Assert::IsTrue(result.task.getDependencies().empty());
	}

	void getsCorrectTypeForEditQueries3() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2(Mocks::Queries::EDIT_QUERY);
		query2.description = boost::none;
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
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.complete.get(),
			result.task.isCompleted());
		Assert::IsTrue(result.task.getSubtasks().empty());
		Assert::IsTrue(result.task.getDependencies().empty());
	}

	void getsCorrectTypeForEditQueries4() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2(Mocks::Queries::EDIT_QUERY);
		query2.priority = boost::none;
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
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.complete.get(),
			result.task.isCompleted());
		Assert::IsTrue(result.task.getSubtasks().empty());
		Assert::IsTrue(result.task.getDependencies().empty());
	}

	void getsCorrectTypeForEditQueries5() {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2(Mocks::Queries::EDIT_QUERY);
		query2.complete = boost::none;
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
		Assert::AreEqual(Mocks::Queries::EDIT_QUERY.deadline.get(),
			result.task.getDeadline());
		Assert::AreEqual(taskList.front().isCompleted(),
			result.task.isCompleted());
		Assert::IsTrue(result.task.getSubtasks().empty());
		Assert::IsTrue(result.task.getDependencies().empty());
	}

	void getsCorrectTypeForEditQueries6() {
		Mocks::TaskList taskList(2);
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2;
		query2.taskID = Mocks::Queries::EDIT_QUERY.taskID;
		query2.childTask = 1;
		You::NLP::QUERY query(query2);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		EDIT_RESULT result(
			boost::get<EDIT_RESULT>(executor->execute()));

		Assert::AreEqual(taskList.front().getID(),
			result.task.getID());
		Assert::IsFalse(result.task.getSubtasks().empty());
		Assert::IsTrue(result.task.getDependencies().empty());
	}

	void getsCorrectTypeForEditQueries7() {
		Mocks::TaskList taskList(2);
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::EDIT_QUERY query2;
		query2.taskID = Mocks::Queries::EDIT_QUERY.taskID;
		query2.dependingTask = 1;
		You::NLP::QUERY query(query2);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		EDIT_RESULT result(
			boost::get<EDIT_RESULT>(executor->execute()));

		// We edit the second element in our task list.
		taskList.erase(begin(taskList), begin(taskList));
		Assert::AreNotEqual(taskList.front().getID(),
			result.task.getID());
		Assert::IsTrue(result.task.getSubtasks().empty());
		Assert::IsFalse(result.task.getDependencies().empty());
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

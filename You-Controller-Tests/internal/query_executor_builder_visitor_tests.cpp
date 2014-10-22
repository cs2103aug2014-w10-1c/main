//@author A0097630B
#include "stdafx.h"
#include <unordered_map>
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
		Mocks::TaskList taskList(5);
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::SHOW_QUERY);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		SHOW_RESULT result(
			boost::get<SHOW_RESULT>(executor->execute()));

		Assert::IsTrue(
			std::is_sorted(begin(result.tasks), end(result.tasks),
			[](const Task& left, const Task& right) {
				return left.getDeadline() >= right.getDeadline();
		}));

		{  // NOLINT(whitespace/braces)
			You::NLP::SHOW_QUERY templ = Mocks::Queries::SHOW_QUERY;
			templ.order = {
				{
					You::NLP::TaskField::DESCRIPTION,
					You::NLP::SHOW_QUERY::Order::ASCENDING
				}
			};
			query = std::move(templ);
		}
		executor = boost::apply_visitor(visitor, query);
		result = boost::get<SHOW_RESULT>(executor->execute());

		Assert::IsTrue(
			std::is_sorted(begin(result.tasks), end(result.tasks),
			[](const Task& left, const Task& right) {
			return left.getDescription() <= right.getDescription();
		}));

		{  // NOLINT(whitespace/braces)
			You::NLP::SHOW_QUERY templ = Mocks::Queries::SHOW_QUERY;
			templ.order = {
				{
					You::NLP::TaskField::PRIORITY,
					You::NLP::SHOW_QUERY::Order::DESCENDING
				}
			};
			query = std::move(templ);
		}
		executor = boost::apply_visitor(visitor, query);
		result = boost::get<SHOW_RESULT>(executor->execute());

		Assert::IsTrue(
			std::is_sorted(begin(result.tasks), end(result.tasks),
			[](const Task& left, const Task& right) {
			return left.getPriority() >= right.getPriority();
		}));
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
		Assert::AreEqual(nlpPriorityToTaskPriority(
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
		Assert::AreEqual(nlpPriorityToTaskPriority(
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
		Assert::AreEqual(nlpPriorityToTaskPriority(
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

private:
	Task::Priority nlpPriorityToTaskPriority(NLP::TaskPriority priority) {
		auto iterator = nlpPriorityToTaskPriorityMap.find(priority);
		assert(iterator != end(nlpPriorityToTaskPriorityMap));
		return iterator->second;
	}

private:
	static const std::unordered_map<NLP::TaskPriority, Task::Priority>
		nlpPriorityToTaskPriorityMap;
};

const std::unordered_map<NLP::TaskPriority, Task::Priority>
QueryExecutorBuilderVisitorTests::nlpPriorityToTaskPriorityMap({
	{ NLP::TaskPriority::NORMAL, Task::Priority::NORMAL },
	{ NLP::TaskPriority::HIGH, Task::Priority::HIGH }
});

}  // namespace UnitTests
}  // namespace Internal
}  // namespace Controller
}  // namespace You

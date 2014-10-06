//@author A0097630B
#include "stdafx.h"
#include "internal/query_executor.h"
#include "internal/query_executor_builder_visitor.h"

#include "../mocks/task_list.h"
#include "../mocks/query.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace Internal {
namespace UnitTests {

namespace Mocks {
	// NOLINTNEXTLINE(build/namespaces)
	using namespace You::Controller::UnitTests::Mocks;
}

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
			Mocks::Queries::ADD_QUERY.due.get(),
			result.task.getDeadline());

		You::NLP::ADD_QUERY queryWithoutDeadline(Mocks::Queries::ADD_QUERY);
		queryWithoutDeadline.due =
			You::Utils::Option<boost::posix_time::ptime>();
		query = queryWithoutDeadline;
		executor = boost::apply_visitor(visitor, query);
		result = boost::get<ADD_RESULT>(executor->execute());

		Assert::AreEqual(
			Mocks::Queries::ADD_QUERY.description,
			result.task.getDescription());
	}

	TEST_METHOD(getsCorrectTypeForEditQueries) {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::EDIT_QUERY);
		std::unique_ptr<QueryExecutor> executor(
			boost::apply_visitor(visitor, query));
		EDIT_RESULT result(
			boost::get<EDIT_RESULT>(executor->execute()));

		Assert::AreEqual(taskList.front(),
			result.task);

		You::NLP::EDIT_QUERY queryWithoutDeadline(Mocks::Queries::EDIT_QUERY);
		queryWithoutDeadline.due =
			You::Utils::Option<boost::posix_time::ptime>();
		query = queryWithoutDeadline;
		executor = boost::apply_visitor(visitor, query);
		result = boost::get<EDIT_RESULT>(executor->execute());

		Assert::AreEqual(taskList.front(),
			result.task);

		queryWithoutDeadline = Mocks::Queries::EDIT_QUERY;
		queryWithoutDeadline.description =
			You::Utils::Option<std::wstring>();
		query = queryWithoutDeadline;
		executor = boost::apply_visitor(visitor, query);
		result = boost::get<EDIT_RESULT>(executor->execute());

		Assert::AreEqual(taskList.front(),
			result.task);
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
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace Controller
}  // namespace You

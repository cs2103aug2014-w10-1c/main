//@author A0097630B
#include "stdafx.h"
#include "internal/query_executor.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace Internal {
namespace UnitTests {

/// A dummy query executor which works like a AddQueryExecutor. This is just
/// for testing.
class DummyQueryExecutor : public QueryExecutor {
	friend class QueryExecutorTests;

public:
	/// Constructor. This creates an executor from a query returned by
	/// the Query Engine.
	explicit DummyQueryExecutor(
		std::unique_ptr<QueryEngine::Query>&& query)
		: QueryExecutor(std::move(query)) {
	}

	virtual ~DummyQueryExecutor() = default;

protected:
	Result processResponse(
		const You::QueryEngine::Response& response) override {
		return ADD_RESULT {
			boost::get<Task>(response)
		};
	}
};

TEST_CLASS(QueryExecutorTests) {
	TEST_METHOD(storesQuery) {
		std::unique_ptr<QueryEngine::Query> query(
			QueryEngine::GetTask(QueryEngine::Filter::anyTask()));

		DummyQueryExecutor executor(std::move(query));
		Assert::IsNotNull(executor.query.get());
	}

	TEST_METHOD(addTasksGetsTask) {
		std::unique_ptr<QueryEngine::Query> query(
			QueryEngine::AddTask(
				L"yay",
				QueryEngine::Task::DEFAULT_START_TIME,
				QueryEngine::Task::DEFAULT_DEADLINE,
				QueryEngine::Task::DEFAULT_PRIORITY,
				{},
				{}));

		DummyQueryExecutor executor(std::move(query));
		ADD_RESULT result = boost::get<ADD_RESULT>(executor.execute());
		Assert::AreEqual(std::wstring(L"yay"), result.task.getDescription());
	}
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace Controller
}  // namespace You

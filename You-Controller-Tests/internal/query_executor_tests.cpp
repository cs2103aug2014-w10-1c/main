//@author A0097630B
#include "stdafx.h"
#include "../internal/test_helpers.h"
#include "internal/query_executor.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace Internal {
namespace UnitTests {

class DummyQueryExecutor : public QueryExecutor {
	friend class QueryExecutorTests;

public:
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
			QueryEngine::FilterTask(QueryEngine::Filter::anyTask())
		);

		DummyQueryExecutor executor(std::move(query));
		Assert::IsNotNull(executor.query.get());
	}
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace Controller
}  // namespace You

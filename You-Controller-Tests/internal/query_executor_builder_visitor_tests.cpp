//@author A0097630B
#include "stdafx.h"
#include "../internal/test_helpers.h"
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
	using namespace You::Controller::UnitTests::Mocks;  // NOLINT
}

TEST_CLASS(QueryExecutorBuilderVisitorTests) {
	TEST_METHOD(getsCorrectTypeForAddQueries) {
		Mocks::TaskList taskList;
		QueryExecutorBuilderVisitor visitor(taskList);

		You::NLP::QUERY query(Mocks::Queries::ADD_QUERY);
		boost::apply_visitor(visitor, query);
	}
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace Controller
}  // namespace You

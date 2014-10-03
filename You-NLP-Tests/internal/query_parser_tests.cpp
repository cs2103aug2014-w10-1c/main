#include "stdafx.h"
#include "You-QueryEngine/task_model.h"
#include "internal/query_parser.h"
#include "../internal/test_helpers.h"
#include "exception.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace Internal {
namespace UnitTests {

using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::hours;
using You::NLP::ParserException;

using You::NLP::Internal::QueryParser;
using You::NLP::Internal::ADD_QUERY;
using You::NLP::Internal::QUERY;

TEST_CLASS(QueryParserTests) {
public:
	TEST_METHOD(throwsExceptionOnEmptyString) {
		Assert::ExpectException<ParserException>([]() {
			QueryParser::parse(L"");
		}, L"Throws exception on empty string");
	}

	TEST_METHOD(throwsExceptionWhenParseFails) {
		Assert::ExpectException<ParserException>([]() {
				// "throw" is currently not defined, so this should work.
				QueryParser::parse(L"/throw");
			}, L"Throws exception on syntax error");
	}

	TEST_METHOD(parsesStringAsTask) {
		QUERY q = QueryParser::parse(L"win");

		Assert::AreEqual(QUERY(ADD_QUERY{
			L"win",
			QueryEngine::Task::DEFAULT_DEADLINE
		}), q);
	}

	TEST_METHOD(parsesStringWithDeadlineAsTask) {
		QUERY q = QueryParser::parse(L"win by may 2014");

		Assert::AreEqual(QUERY(ADD_QUERY{
			L"win",
			ptime(date(2014, boost::gregorian::May, 1), hours(0))
		}), q);
	}
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace NLP
}  // namespace You

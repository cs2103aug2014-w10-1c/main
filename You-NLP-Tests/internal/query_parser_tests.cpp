#include "stdafx.h"
#include "internal/query_parser.h"
#include "../internal/test_helpers.h"
#include "exception.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

using boost::gregorian::date;
using You::NLP::ParserException;

using You::NLP::Internal::QueryParser;
using You::NLP::Internal::ADD_QUERY;
using You::NLP::Internal::QUERY;

TEST_CLASS(QueryParserTests) {
public:
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
			date()
		}), q);
	}

	TEST_METHOD(parsesStringWithDeadlineAsTask) {
		QUERY q = QueryParser::parse(L"win by 23 may");

		Assert::AreEqual(QUERY(ADD_QUERY{
			L"win",
			date(0, boost::gregorian::May, 23)
		}), q);
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

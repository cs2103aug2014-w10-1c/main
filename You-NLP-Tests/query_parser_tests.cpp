//@author A0097630B
#include "stdafx.h"
#include "You-QueryEngine/task_model.h"

#include "exception.h"
#include "query_parser.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::hours;
using You::NLP::ParserException;

using You::NLP::QueryParser;
using You::NLP::ADD_QUERY;
using You::NLP::EDIT_QUERY;
using You::NLP::QUERY;

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

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"win"
		}), q);
	}

	TEST_METHOD(parsesStringWithDeadlineAsTask) {
		QUERY q = QueryParser::parse(L"win by may 2014");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"win",
			ptime(date(2014, boost::gregorian::May, 1), hours(0))
		}), q);
	}

	TEST_METHOD(parsesEditQuery) {
		QUERY q = QueryParser::parse(L"/edit 10 set description meh");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			L"meh"
		}), q);

		q = QueryParser::parse(L"/edit 10 set due oct 2014");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			Utils::Option<std::wstring>(),
			ptime(date(2014, boost::gregorian::Oct, 1), hours(0))
		}), q);

		q = QueryParser::parse(L"/edit 10 set complete");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10
		}), q);
	}

	TEST_METHOD(parsesDeleteQuery) {
		QUERY q = QueryParser::parse(L"/delete 10");

		Assert::AreEqual(QUERY(DELETE_QUERY {
			10
		}), q);
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

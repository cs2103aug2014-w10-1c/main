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

		q = QueryParser::parse(L"win lottery");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"win lottery"
		}), q);
	}

	TEST_METHOD(parsesStringWithDeadlineAsTask) {
		QUERY q = QueryParser::parse(L"win by may 2014");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"win",
			TaskPriority::NORMAL,
			ptime(date(2014, boost::gregorian::May, 1), hours(0))
		}), q);

		q = QueryParser::parse(L"win lottery by dec 2014");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"win lottery",
			TaskPriority::NORMAL,
			ptime(date(2014, boost::gregorian::Dec, 1), hours(0))
		}), q);
	}

	TEST_METHOD(parsesStringWithPriorityAsTask) {
		QUERY q = QueryParser::parse(L"win!");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"win",
			TaskPriority::HIGH
		}), q);

		q = QueryParser::parse(L"win lottery! by dec 2014");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"win lottery",
			TaskPriority::HIGH,
			ptime(date(2014, boost::gregorian::Dec, 1), hours(0))
		}), q);
	}

	TEST_METHOD(parsesShowQuery) {
		// Boundary case: one filter, zero sort.
		QUERY q = QueryParser::parse(L"/show description='\\\\\\'meh'");

		Assert::AreEqual(QUERY(SHOW_QUERY {
			{
				{
					TaskField::DESCRIPTION,
					SHOW_QUERY::Predicate::EQ,
					L"\\\'meh"
				}
			},
			{}
		}), q);

		// Boundary case: more than one filter, zero sort.
		q = QueryParser::parse(L"/show description!='\\\\\\'meh', "
			L"priority<\'high\'");

		Assert::AreEqual(QUERY(SHOW_QUERY {
				{
					{
						TaskField::DESCRIPTION,
						SHOW_QUERY::Predicate::NOT_EQ,
						L"\\\'meh"
					},
					{
						TaskField::PRIORITY,
						SHOW_QUERY::Predicate::LESS_THAN,
						L"high"
					}
				},
				{}
		}), q);

		// Boundary case: zero filter, one sort.
		q = QueryParser::parse(L"/show order by description ascending");

		Assert::AreEqual(QUERY(SHOW_QUERY {
			{},
			{ { TaskField::DESCRIPTION, SHOW_QUERY::Order::ASCENDING } }
		}), q);

		// Boundary case: zero filter, more than one sort.
		q = QueryParser::parse(L"/show order by description descending, "
			L"priority");

		Assert::AreEqual(QUERY(SHOW_QUERY {
			{},
			{
				{ TaskField::DESCRIPTION, SHOW_QUERY::Order::DESCENDING },
				{ TaskField::PRIORITY, SHOW_QUERY::Order::ASCENDING }
			}
		}), q);

		// Boundary case: nonzero filter, nonzero sort.
		q = QueryParser::parse(L"/show description!='\\\\\\'meh', "
			L"priority<\'high\' order by description descending, "
			L"priority");

		Assert::AreEqual(QUERY(SHOW_QUERY {
			{
				{
					TaskField::DESCRIPTION,
					SHOW_QUERY::Predicate::NOT_EQ,
					L"\\\'meh"
				},
				{
					TaskField::PRIORITY,
					SHOW_QUERY::Predicate::LESS_THAN,
					L"high"
				}
			},
			{
				{ TaskField::DESCRIPTION, SHOW_QUERY::Order::DESCENDING },
				{ TaskField::PRIORITY, SHOW_QUERY::Order::ASCENDING }
			}
		}), q);
	}

	TEST_METHOD(parsesEditQuery) {
		QUERY q = QueryParser::parse(L"/edit 10 set description meh");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			L"meh"
		}), q);

		q = QueryParser::parse(L"/edit 10 set description meh with spaces");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			L"meh with spaces"
		}), q);

		q = QueryParser::parse(L"/edit 10 set deadline oct 2014");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			Utils::Option<std::wstring>(),
			Utils::Option<TaskPriority>(),
			ptime(date(2014, boost::gregorian::Oct, 1), hours(0))
		}), q);

		q = QueryParser::parse(L"/edit 10 set complete");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			Utils::Option<std::wstring>(),
			Utils::Option<TaskPriority>(),
			Utils::Option<boost::posix_time::ptime>(),
			true
		}), q);

		q = QueryParser::parse(L"/edit 10 set priority high");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			Utils::Option<std::wstring>(),
			TaskPriority::HIGH
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

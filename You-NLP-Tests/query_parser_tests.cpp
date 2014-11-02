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
		Assert::ExpectException<ParserException>(
			std::bind(static_cast<QUERY (*)(const std::wstring&)>(
				&QueryParser::parse), L""));
	}

	TEST_METHOD(throwsExceptionWhenParseFails) {
		// "throw" is currently not defined, so this should work.
		Assert::ExpectException<ParserException>(
			std::bind(static_cast<QUERY (*)(const std::wstring&)>(
				&QueryParser::parse), L"/throw"));
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

	TEST_METHOD(parsesStringWithSubtasksAsTask) {
		QUERY q = QueryParser::parse(L"Walk the dog by 20 oct : Eat breakfast "
			L"; Take out the dog; Open the door; Buy a new collar by 12 oct");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"Walk the dog",
			TaskPriority::NORMAL,
			ptime(date(2015, boost::gregorian::Oct, 20)),
			{
				{ L"Eat breakfast" },
				{ L"Take out the dog" },
				{ L"Open the door" },
				{
					L"Buy a new collar",
					TaskPriority::NORMAL,
					ptime(date(2015, boost::gregorian::Oct, 12))
				}
			}
		}), q);
	}

	TEST_METHOD(parsesIrregularSpacingAddTask) {
		QUERY q;
		Assert::ExpectException<ParseErrorException>(
			boost::phoenix::bind(&QueryParser::parse,
				L"/adds Hello World by 20 oct"));

		q = QueryParser::parse(L"/add E by 22 oct");

		Assert::AreEqual(QUERY(ADD_QUERY {
			L"E",
			TaskPriority::NORMAL,
			ptime(date(2015, boost::gregorian::Oct, 22), hours(0))
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
					std::wstring(L"\\\'meh")
				}
			},
			{}
		}), q);

		// Boundary case: more than one filter, zero sort.
		q = QueryParser::parse(L"/show description!='\\\\\\'meh', "
			L"priority<high");

		Assert::AreEqual(QUERY(SHOW_QUERY {
				{
					{
						TaskField::DESCRIPTION,
						SHOW_QUERY::Predicate::NOT_EQ,
						std::wstring(L"\\\'meh")
					},
					{
						TaskField::PRIORITY,
						SHOW_QUERY::Predicate::LESS_THAN,
						TaskPriority::HIGH
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
			L"priority<high, priority>normal, deadline>='3 oct', "
			L"deadline<='7 oct', complete=true "
			L"order by description descending, priority");

		Assert::AreEqual(QUERY(SHOW_QUERY {
			{
				{
					TaskField::DESCRIPTION,
					SHOW_QUERY::Predicate::NOT_EQ,
					std::wstring(L"\\\'meh")
				},
				{
					TaskField::PRIORITY,
					SHOW_QUERY::Predicate::LESS_THAN,
					TaskPriority::HIGH
				},
				{
					TaskField::PRIORITY,
					SHOW_QUERY::Predicate::GREATER_THAN,
					TaskPriority::NORMAL
				},
				{
					TaskField::DEADLINE,
					SHOW_QUERY::Predicate::GREATER_THAN_EQ,
					boost::posix_time::ptime(
						boost::gregorian::date(2015, 10, 3),
						boost::posix_time::hours(0))
				},
				{
					TaskField::DEADLINE,
					SHOW_QUERY::Predicate::LESS_THAN_EQ,
					boost::posix_time::ptime(
						boost::gregorian::date(2015, 10, 7),
						boost::posix_time::hours(0))
				},
				{
					TaskField::COMPLETE,
					SHOW_QUERY::Predicate::EQ,
					true
				}
			},
			{
				{ TaskField::DESCRIPTION, SHOW_QUERY::Order::DESCENDING },
				{ TaskField::PRIORITY, SHOW_QUERY::Order::ASCENDING }
			}
		}), q);
	}

	TEST_METHOD(parsesShowQueryWithWrongType) {
		Assert::ExpectException<ParserTypeException>([]() {
			QueryParser::parse(L"/show description=false");
		});
	}

	TEST_METHOD(parsesEditQuery) {
		QUERY q = QueryParser::parse(L"/edit 10 set description='meh'");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			L"meh"
		}), q);

		q = QueryParser::parse(L"/edit 10 set description='meh with spaces'");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			L"meh with spaces"
		}), q);

		q = QueryParser::parse(L"/edit 10 set deadline='oct 2014'");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			boost::none,
			boost::none,
			ptime(date(2014, boost::gregorian::Oct, 1), hours(0))
		}), q);

		q = QueryParser::parse(L"/edit 10 set complete");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			boost::none,
			boost::none,
			boost::none,
			true
		}), q);

		q = QueryParser::parse(L"/edit 10 set priority=high");

		Assert::AreEqual(QUERY(EDIT_QUERY {
			10,
			boost::none,
			TaskPriority::HIGH
		}), q);
	}

	TEST_METHOD(parsesEditQueryWithWrongType) {
		Assert::ExpectException<ParserTypeException>(
			std::bind(static_cast<QUERY (*)(const std::wstring&)>(
				&QueryParser::parse),
				L"/edit 10 set description='14 oct'"));
	}

	TEST_METHOD(parsesDeleteQuery) {
		QUERY q = QueryParser::parse(L"/delete 10");

		Assert::AreEqual(QUERY(DELETE_QUERY {
			10
		}), q);

		Assert::ExpectException<ParseErrorException>(std::bind(
			static_cast<QUERY (*)(const std::wstring&)>(
				&QueryParser::parse), L"/delete10"));
	}

	TEST_METHOD(parsesUndoQuery) {
		QUERY q = QueryParser::parse(L"/undo");

		Assert::AreEqual(QUERY(UNDO_QUERY {}), q);
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

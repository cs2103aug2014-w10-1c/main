//@author A0097630B
#include "stdafx.h"
#include "exception.h"
#include "date_time_parser.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::hours;

using You::NLP::DateTimeParser;

TEST_CLASS(DateTimeParserTests) {
public:
	TEST_METHOD(rejectsEmptyString) {
		Assert::ExpectException<ParserException>(
			std::bind(static_cast<ptime (*)(const std::wstring&)>(
				&DateTimeParser::parse), L""),
			L"Throws exception on empty string");
	}

	TEST_METHOD(acceptsTrailingWhitespace) {
		DateTimeParser::parse(L"13 nov			");
	}

	TEST_METHOD(parsesYear) {
		Assert::AreEqual(2056,
			static_cast<int>(DateTimeParser::parse(L"2056").date().year()));
	}

	TEST_METHOD(parsesTwoDigitYear) {
		Assert::AreEqual(static_cast<int>(parseTwoDigitYear(13)),
			static_cast<int>(DateTimeParser::parse(L"13").date().year()));
		Assert::AreEqual(static_cast<int>(parseTwoDigitYear(45)),
			static_cast<int>(DateTimeParser::parse(L"45").date().year()));
	}

	TEST_METHOD(parsesStringMonths) {
		std::array<std::wstring, 12> inputs = {
			L"2014-Jan",
			L"2014-Feb",
			L"2014-Mar",
			L"2014-Apr",
			L"2014-May",
			L"2014-Jun",
			L"2014-Jul",
			L"2014-Aug",
			L"2014-Sep",
			L"2014-Oct",
			L"2014-Nov",
			L"2014-Dec"
		};

		for (auto i = inputs.begin(); i != inputs.end(); ++i) {
			const std::wstring& str = *i;
			Assert::AreEqual(i - inputs.begin() + 1,
				static_cast<int>(DateTimeParser::parse(str).date().month()));
		}
	}

	TEST_METHOD(parsesStringMonthsCaseInsensitive) {
		Assert::AreEqual(9, static_cast<int>(
			DateTimeParser::parse(L"2014-sep").date().month()));
	}

	TEST_METHOD(parsesYearWithMonthAsFirstDay) {
		date d = DateTimeParser::parse(L"2014-May").date();

		Assert::AreEqual(2014, static_cast<int>(d.year()));
		Assert::AreEqual(static_cast<int>(boost::gregorian::May),
			static_cast<int>(d.month()));
		Assert::AreEqual(1, static_cast<int>(d.day()));
	}

	TEST_METHOD(parsesMonthWithYear) {
		date d = DateTimeParser::parse(L"may 2014").date();

		Assert::AreEqual(2014, static_cast<int>(d.year()));
		Assert::AreEqual(static_cast<int>(boost::gregorian::May),
			static_cast<int>(d.month()));
		Assert::AreEqual(1, static_cast<int>(d.day()));
	}

	TEST_METHOD(parsesYearMonthDay) {
		date d = DateTimeParser::parse(L"2014-Oct-7").date();
		Assert::AreEqual(date(2014, boost::gregorian::Oct, 7), d);

		d = DateTimeParser::parse(L"2014-11-7").date();
		Assert::AreEqual(date(2014, boost::gregorian::Nov, 7), d);
	}

	TEST_METHOD(parsesDayWithMonth) {
		date d = boost::posix_time::second_clock::local_time().date();
		date actualTomorrow = d + boost::gregorian::days(1);

		date tomorrow = DateTimeParser::parse((
			boost::wformat(L"%1% %2%") %
				actualTomorrow.day() %
				actualTomorrow.month()).str()).date();
		Assert::AreEqual(actualTomorrow, tomorrow);

		date nextYear = d + boost::gregorian::years(1) -
			boost::gregorian::days(1);
		date yesterday = DateTimeParser::parse((
			boost::wformat(L"%1% %2%") %
			nextYear.day() %
			nextYear.month()).str()).date();
		Assert::AreEqual(nextYear, yesterday);
	}

	TEST_METHOD(parsesRelativeMonths) {
		date today = boost::posix_time::second_clock::local_time().date();
		date nextOctober = DateTimeParser::parse(L"next oct").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::months_of_year::Oct),
			static_cast<int>(nextOctober.month()));
		Assert::AreEqual(static_cast<unsigned short>(1),
			nextOctober.day().as_number());
		Assert::IsTrue(nextOctober > today);

		date lastOctober = DateTimeParser::parse(L"last oct").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::months_of_year::Oct),
			static_cast<int>(lastOctober.month()));
		Assert::AreEqual(static_cast<unsigned short>(1),
			lastOctober.day().as_number());
		Assert::IsTrue(lastOctober < today);

		date thisNovember = DateTimeParser::parse(L"this nov").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::months_of_year::Nov),
			static_cast<int>(thisNovember.month()));
		Assert::IsTrue(thisNovember > today);
		Assert::AreEqual(static_cast<unsigned short>(1),
			thisNovember.day().as_number());
		Assert::IsTrue((thisNovember - today).days() <= 366);

		date thisDecember = DateTimeParser::parse(L"this dec").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::months_of_year::Dec),
			static_cast<int>(thisDecember.month()));
		Assert::IsTrue(thisDecember > today);
		Assert::AreEqual(static_cast<unsigned short>(1),
			thisDecember.day().as_number());
		Assert::IsTrue((thisDecember - today).days() <= 366);

		date thisOctober = DateTimeParser::parse(L"this oct").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::months_of_year::Oct),
			static_cast<int>(thisOctober.month()));
		Assert::IsTrue(thisOctober > today);
		Assert::AreEqual(static_cast<unsigned short>(1),
			thisOctober.day().as_number());
		Assert::IsTrue((thisOctober - today).days() <= 366);
	}

	TEST_METHOD(parsesRelativeDaysOfWeek) {
		date today = boost::posix_time::second_clock::local_time().date();
		date nextMonday = DateTimeParser::parse(L"next monday").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::weekdays::Monday),
			static_cast<int>(nextMonday.day_of_week()));
		Assert::IsTrue(nextMonday > today);
		Assert::IsTrue((nextMonday - today).days() <= 13);

		date lastMonday = DateTimeParser::parse(L"last monday").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::weekdays::Monday),
			static_cast<int>(lastMonday.day_of_week()));
		Assert::IsTrue(lastMonday < today);
		Assert::IsTrue((today - lastMonday).days() <= 13);

		date thisSaturday = DateTimeParser::parse(L"this sat").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::weekdays::Saturday),
			static_cast<int>(thisSaturday.day_of_week()));
		Assert::IsTrue(thisSaturday > today);
		Assert::IsTrue((thisSaturday - today).days() <= 7);

		date thisSunday = DateTimeParser::parse(L"this sun").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::weekdays::Sunday),
			static_cast<int>(thisSunday.day_of_week()));
		Assert::IsTrue(thisSunday > today);
		Assert::IsTrue((thisSunday - today).days() <= 7);

		date thisFriday = DateTimeParser::parse(L"this fri").date();
		Assert::AreEqual(
			static_cast<int>(boost::date_time::weekdays::Friday),
			static_cast<int>(thisFriday.day_of_week()));
		Assert::IsTrue(thisFriday > today);
		Assert::IsTrue((thisFriday - today).days() <= 7);
	}

	TEST_METHOD(parsesTomorrow) {
		date today = boost::posix_time::second_clock::local_time().date();
		date tomorrow = today + boost::gregorian::days(1);

		Assert::AreEqual(tomorrow, DateTimeParser::parse(L"tomorrow").date());
	}

	TEST_METHOD(parsesYesterday) {
		date today = boost::posix_time::second_clock::local_time().date();
		date yesterday = today - boost::gregorian::days(1);

		Assert::AreEqual(yesterday, DateTimeParser::parse(L"yesterday").date());
	}

	TEST_METHOD(parsesRelativeToN) {
		date today = boost::posix_time::second_clock::local_time().date();
		date next = today + boost::gregorian::days(3);

		Assert::AreEqual(next, DateTimeParser::parse(L"3 days").date());

		next = today + boost::gregorian::days(14);
		Assert::AreEqual(next, DateTimeParser::parse(L"2 weeks").date());
	}

private:
	boost::gregorian::greg_year parseTwoDigitYear(DateTimeParser::Year year) {
		return DateTimeParser::parseTwoDigitYear(year);
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

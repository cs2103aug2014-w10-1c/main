#include "stdafx.h"
#include "internal/date_time_parser.h"
#include "../internal/test_helpers.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace Internal {
namespace UnitTests {

using boost::gregorian::date;
using boost::posix_time::ptime;
using boost::posix_time::hours;

using You::NLP::Internal::DateTimeParser;

TEST_CLASS(DateTimeParserTests) {
public:
	TEST_METHOD(parsesYear) {
		Assert::AreEqual(static_cast<short>(2056),
			static_cast<short>(DateTimeParser::parse(L"2056").date().year()));
	}

	TEST_METHOD(parsesTwoDigitYear) {
		Assert::AreEqual(static_cast<short>(parseTwoDigitYear(13)),
			static_cast<short>(DateTimeParser::parse(L"13").date().year()));
	}

	TEST_METHOD(parsesYearWithMonthAsFirstDay) {
		date d = DateTimeParser::parse(L"2014-May").date();

		Assert::AreEqual(2014, static_cast<int>(d.year()));
		Assert::AreEqual(static_cast<int>(boost::gregorian::May),
			static_cast<int>(d.month()));
		Assert::AreEqual(1, static_cast<int>(d.day()));
	}

private:
	boost::gregorian::greg_year parseTwoDigitYear(int16_t year) {
		return DateTimeParser::parseTwoDigitYear(year);
	}
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace NLP
}  // namespace You

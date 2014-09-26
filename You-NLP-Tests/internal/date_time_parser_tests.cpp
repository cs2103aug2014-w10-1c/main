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

private:
	int parseTwoDigitYear(int year) {
		return DateTimeParser::parseTwoDigitYearNative(year);
	}
};

}  // namespace UnitTests
}  // namespace Internal
}  // namespace NLP
}  // namespace You

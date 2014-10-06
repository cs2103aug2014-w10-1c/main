#include "stdafx.h"
#include "query.h"

namespace You {
namespace Controller {
namespace UnitTests {
namespace Mocks {
namespace Queries {

extern const You::NLP::ADD_QUERY ADD_QUERY {
	L"wow",
	boost::posix_time::ptime(
		boost::gregorian::date(2014, boost::gregorian::Oct, 1),
		boost::posix_time::hours(1) + boost::posix_time::minutes(1)
	)
};

}  // namespace Queries
}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

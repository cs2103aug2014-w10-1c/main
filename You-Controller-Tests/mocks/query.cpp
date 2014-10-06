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
		boost::posix_time::hours(0)
	)
};

extern const You::NLP::EDIT_QUERY EDIT_QUERY {
	0,
	L"new description",
	boost::posix_time::ptime(
		boost::gregorian::date(2014, boost::gregorian::Oct, 1),
		boost::posix_time::hours(1) + boost::posix_time::minutes(1)
	)
};

extern const You::NLP::DELETE_QUERY DELETE_QUERY {
	0
};

}  // namespace Queries
}  // namespace Mocks
}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

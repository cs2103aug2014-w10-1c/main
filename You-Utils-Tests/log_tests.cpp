//@author A0097630B
#include "stdafx.h"
#include "log.h"
#include <boost/phoenix/phoenix.hpp>

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

namespace You {
namespace Utils {
namespace UnitTests {

TEST_CLASS(LogTests) {
	TEST_METHOD(debugLogMethod) {
		Log::debug << L"lol";
	}
};

}  // namespace UnitTests
}  // namespace Utils
}  // namespace You

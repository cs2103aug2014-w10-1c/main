//@author A0097630B
#include "stdafx.h"
#include "internal/test_helpers.h"
#include "controller.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

std::wstring ToString(const You::Controller::Controller& value) {
	return ToString(static_cast<const void*>(&value));
}

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

namespace You {
namespace Controller {
namespace UnitTests {

TEST_CLASS(ControllerTests) {
	TEST_METHOD(controllerIsSingleton) {
		Assert::AreSame(
			Controller::get(),
			Controller::get());
	}
};

}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

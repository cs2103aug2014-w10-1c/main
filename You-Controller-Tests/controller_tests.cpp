//@author A0097630B
#include "stdafx.h"
#include "internal/test_helpers.h"
#include "controller.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace UnitTests {

TEST_CLASS(ControllerTests) {
	TEST_METHOD(controllerIsSingleton) {
		Assert::AreSame(
			reinterpret_cast<uintptr_t>(&Controller::get()),
			reinterpret_cast<uintptr_t>(&Controller::get()));
	}
};

}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

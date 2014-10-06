//@author A0097630B
#include "stdafx.h"
#include "internal/test_helpers.h"
#include "controller_context.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace UnitTests {

TEST_CLASS(ControllerContextTests) {
public:
	TEST_METHOD(defaultIsADefaultContext) {
		Assert::IsTrue(Controller::Context::DEFAULT.isDefault());
	}

	TEST_METHOD(taskListIsNotADefaultContext) {
		TaskList list;
		Controller::Context controllerContext(list);
		Assert::IsFalse(controllerContext.isDefault());
	}
};

}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

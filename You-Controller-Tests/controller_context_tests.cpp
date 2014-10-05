//@author A0097630B
#include "stdafx.h"
#include "controller_context.h"
#include "exception.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace UnitTests {

using You::NLP::ContextRequiredException;

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

	TEST_METHOD(throwsContextRequiredExceptionWhenDefaultIsUsed) {
		Assert::ExpectException<ContextRequiredException>([]() {
			Controller::Context::DEFAULT.at(0);
		}, L"Default context throws exception when used.");

		Assert::ExpectException<ContextRequiredException>([]() {
			Controller::Context::DEFAULT[0];
		}, L"Default context throws exception when used.");
	}

	TEST_METHOD(atThrowsOutOfRangeWhenUsedWithInvalidIndex) {
		TaskList list;
		Controller::Context controllerContext(list);

		Assert::ExpectException<std::out_of_range>([&]() {
			controllerContext.at(0);
		}, L"at throws exception when used with invalid index.");
	}

	TEST_METHOD(atAndArrayAccessReturnsCorrectItem) {
		
	}
};

}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

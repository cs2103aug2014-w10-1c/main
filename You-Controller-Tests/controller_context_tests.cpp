//@author A0097630B
#include "stdafx.h"
#include "exception.h"
#include "controller_context.h"
#include "mocks/task_list.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace UnitTests {

using You::Controller::ContextRequiredException;

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
		Mocks::TaskList taskList;
		Controller::Context context(taskList);

		// TODO(lowjoel): Use AreEqual when there's an accessible ToString
		Assert::IsTrue(taskList.front() == context[0]);
		Assert::IsTrue(taskList.front() == context.at(0));
	}
};

}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

//@author A0097630B
#include "stdafx.h"
#include "internal/test_helpers.h"
#include "controller.h"
#include "controller_context.h"

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

	TEST_METHOD(query) {
		ADD_RESULT result = boost::get<ADD_RESULT>(
			Controller::get().query(L"/add hello",
			Controller::Context::DEFAULT));

		Assert::AreEqual(std::wstring(L"hello"), result.task.getDescription());
	}

	TEST_METHOD(getAllTasks) {
		Controller::get().getTasks();
	}

	TEST_METHOD(getSomeTasks) {
		Controller::get().getTasks({});
	}
};

}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

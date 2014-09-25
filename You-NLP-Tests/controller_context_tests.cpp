#include "stdafx.h"
#include "internal/test_helpers.h"
#include "controller_context.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

TEST_CLASS(ControllerContextTests) {
public:
	TEST_METHOD(defaultIsADefaultContext) {
		Assert::IsTrue(Controller::Context::DEFAULT.isDefault());
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

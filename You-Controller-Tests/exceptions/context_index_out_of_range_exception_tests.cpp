//@author A0097630B
#include "stdafx.h"
#include "exceptions/context_index_out_of_range_exception.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Controller {
namespace UnitTests {

TEST_CLASS(ContextIndexOutOfRangeExceptionTests) {
public:
	TEST_METHOD(contextIndexOutOfRangeExceptionIsAControllerException) {
		Assert::ExpectException<Exception>([]() {
			throw ContextIndexOutOfRangeException();
		});
	}

	TEST_METHOD(contextIndexOutOfRangeExceptionIsAnOutOfRange) {
		Assert::ExpectException<std::out_of_range>([]() {
			throw ContextIndexOutOfRangeException();
		});
	}
};

}  // namespace UnitTests
}  // namespace Controller
}  // namespace You

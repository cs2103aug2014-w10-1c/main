/// Project-level integration tests
#include "stdafx.h"
#include <CppUnitTest.h>

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace Tests {
	TEST_CLASS(NlpTests) {
	public:
		TEST_METHOD(DummyTest1) {
			Assert::IsTrue(true);
		}
	};
}  // namespace Tests
}  // namespace NLP
}  // namespace You

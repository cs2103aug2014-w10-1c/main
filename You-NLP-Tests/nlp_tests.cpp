/// Project-level integration tests
#include "stdafx.h"
#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace You {
namespace NLP {
namespace Tests {
	TEST_CLASS(NlpTests) {
	public:
		TEST_METHOD(DummyTest1) {
			Assert::IsTrue(false);
		}
	};
}
}
}

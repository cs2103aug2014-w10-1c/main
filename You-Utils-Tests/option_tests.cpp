//@author A0097630B
#include "stdafx.h"
#include "option.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace Microsoft {
namespace VisualStudio {
namespace CppUnitTestFramework {

template<typename T>
std::wstring ToString(const You::Utils::Option<T>& value) {
	if (value) {
		return ToString(value.get());
	} else {
		return L"(none)";
	}
}

}  // namespace CppUnitTestFramework
}  // namespace VisualStudio
}  // namespace Microsoft

namespace You {
namespace Utils {
namespace UnitTests {

TEST_CLASS(OptionTests) {
	TEST_METHOD(defaultConstructorConstructsFalseyObject) {
		Assert::IsFalse(Option<int>());
	}

	TEST_METHOD(constructorWithFalseyValueIsTrue) {
		Assert::IsTrue(Option<int>(0));
	}

	TEST_METHOD(getsValidValue) {
		Assert::AreEqual(42, Option<int>(42).get());

		const Option<int> test(43);
		Assert::AreEqual(43, test.get());
	}

	TEST_METHOD(comparesEquality) {
		const Option<int> blank;
		const Option<int> something(42);

		Assert::AreEqual(blank, blank);
		Assert::AreEqual(something, something);
		Assert::AreNotEqual(blank, something);
		Assert::AreNotEqual(something, blank);
	}
};

}  // namespace UnitTests
}  // namespace Utils
}  // namespace you

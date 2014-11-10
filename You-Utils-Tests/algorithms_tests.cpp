//@author A0097630B
#include "stdafx.h"
#include "string_utils.h"
#include "algorithms.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace Utils {
namespace UnitTests {

TEST_CLASS(StringSimilarityTests) {
	/// Tests the class for both narrow and wide strings.
	template<typename T>
	void testContainsImpl(const T& lhs, const T& rhs, bool expect) {
		Assert::AreEqual(expect, similar_to<T>()(lhs, rhs));
	}

	/// Tests the class from narrow string input.
	void testContains(const char* lhs, const char* rhs, bool expect) {
		std::wstring lhsWString(boost::lexical_cast<std::wstring>(lhs));
		std::wstring rhsWString(boost::lexical_cast<std::wstring>(rhs));
		std::string lhsString(boost::lexical_cast<std::string>(lhs));
		std::string rhsString(boost::lexical_cast<std::string>(rhs));

		testContainsImpl(lhsWString, rhsWString, expect);
		testContainsImpl(lhsString, rhsString, expect);
	}

	TEST_METHOD(emptyStringDoesNotContain) {
		testContains("", "", false);
	}

	TEST_METHOD(emptySearchDoesNotMatch) {
		testContains("abc", "", false);
	}

	TEST_METHOD(singleWordContains) {
		testContains("ab", "a", true);
		testContains("ab", "b", true);
	}

	TEST_METHOD(singleWordSupersetDoesNotContain) {
		testContains("a", "ab", false);
	}

	TEST_METHOD(multiWordContains) {
		testContains("ab cd", "cd", true);
	}

	TEST_METHOD(searchMustMatchAll) {
		testContains("abc", "ab bc", true);
		testContains("abc", "ab cd", false);
	}

	TEST_METHOD(caseInsensitiveCompare) {
		testContains("abc", "AB BC", true);
	}
};

}  // namespace UnitTests
}  // namespace Utils
}  // namespace You

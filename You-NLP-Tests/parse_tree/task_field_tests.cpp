//@author A0097630B
#include "stdafx.h"
#include "../tests_stdafx.h"
#include "parse_tree/task_field.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

TEST_CLASS(TaskFieldTests) {
	TEST_METHOD(fieldsBinaryOr) {
		TaskField lhs = TaskField::COMPLETE;
		TaskField rhs = TaskField::PRIORITY;

		Assert::AreEqual(
			static_cast<size_t>(lhs) | static_cast<size_t>(rhs),
			static_cast<size_t>(lhs | rhs));
	}

	TEST_METHOD(fieldsBinaryAnd) {
		TaskField lhs = TaskField::COMPLETE;
		TaskField rhs = TaskField::PRIORITY;

		Assert::AreEqual(
			static_cast<size_t>(lhs)& static_cast<size_t>(rhs),
			static_cast<size_t>(lhs & rhs));
	}

	TEST_METHOD(emptyFieldConvertsToEmptyString) {
		Assert::AreEqual(
			std::wstring(),
			boost::lexical_cast<std::wstring>(TaskField::NONE));
	}
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

//@author A0097630B
#include "stdafx.h"
#include "query_parser.h"
#include "../internal/test_helpers.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

using You::NLP::DELETE_QUERY;

TEST_CLASS(DeleteQueryTests) {
public:
	TEST_METHOD(convertsToStream) {
		std::wostringstream stream;
		stream << DUMMY;
		Assert::AreEqual(
			std::wstring(L"Delete task #1"),
			stream.str());
	}

	TEST_METHOD(convertsToString) {
		Assert::AreEqual(
			std::wstring(L"Delete task #1"),
			boost::lexical_cast<std::wstring>(DUMMY));
	}

	TEST_METHOD(comparesEquality) {
		DELETE_QUERY local = DUMMY;

		Assert::AreEqual(DUMMY, local);
	}

	TEST_METHOD(comparesInequality) {
		DELETE_QUERY local{
			2
		};

		Assert::AreNotEqual(DUMMY, local);
	}

private:
	/// A dummy object.
	static const DELETE_QUERY DUMMY;
};

const DELETE_QUERY DeleteQueryTests::DUMMY {
	1
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

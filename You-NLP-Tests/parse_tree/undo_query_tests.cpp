//@author A0097630B
#include "stdafx.h"
#include "parse_tree.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

using You::NLP::UNDO_QUERY;

TEST_CLASS(UndoQueryTests) {
	TEST_METHOD(convertsToStream) {
		std::wostringstream stream;
		stream << DUMMY;
		Assert::AreEqual(
			std::wstring(L"undo"),
			stream.str());
	}

	TEST_METHOD(convertsToString) {
		Assert::AreEqual(
			std::wstring(L"undo"),
			boost::lexical_cast<std::wstring>(DUMMY));
	}

	TEST_METHOD(comparesEquality) {
		UNDO_QUERY local = DUMMY;

		Assert::AreEqual(DUMMY, local);
	}

	TEST_METHOD(comparesInequality) {
		UNDO_QUERY local;

		Assert::IsFalse(!(DUMMY == local));
	}

private:
	/// A dummy object.
	static const UNDO_QUERY DUMMY;
};

const UNDO_QUERY UndoQueryTests::DUMMY;

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You

//@author A0097630B
#include "stdafx.h"
#include "parse_tree/show_query.h"

using Assert = Microsoft::VisualStudio::CppUnitTestFramework::Assert;

namespace You {
namespace NLP {
namespace UnitTests {

TEST_CLASS(ShowQueryTests) {
	TEST_METHOD(convertsToStream) {
		std::wostringstream stream;
		stream << DUMMY;
		Assert::AreEqual(
			std::wstring(L"Show tasks (criteria none, sort by "
				L"Description ascending)"),
			stream.str());
	}

	TEST_METHOD(convertsToString) {
		Assert::AreEqual(
			std::wstring(L"Show tasks (criteria none, sort by "
				L"Description ascending)"),
			boost::lexical_cast<std::wstring>(DUMMY));
	}

	TEST_METHOD(comparesEquality) {
		SHOW_QUERY local = DUMMY;

		Assert::AreEqual(DUMMY, local);
	}

	TEST_METHOD(comparesInequality) {
		SHOW_QUERY local {
			{},
			{
				SHOW_QUERY::FIELD_ORDER {
					TaskField::DESCRIPTION, SHOW_QUERY::Order::ASCENDING
				}
			}
		};

		Assert::AreEqual(DUMMY, local);

		local.order[0].field = TaskField::DEADLINE;
		Assert::AreNotEqual(DUMMY, local);

		SHOW_QUERY local2 = local;
		local2.order.push_back(SHOW_QUERY::FIELD_ORDER {
			TaskField::DESCRIPTION
		});
		Assert::AreNotEqual(DUMMY, local2);
	}

private:
	/// A dummy object.
	static const SHOW_QUERY DUMMY;
};

const SHOW_QUERY ShowQueryTests::DUMMY {
	{},
	{ { TaskField::DESCRIPTION, SHOW_QUERY::Order::ASCENDING } }
};

}  // namespace UnitTests
}  // namespace NLP
}  // namespace You
